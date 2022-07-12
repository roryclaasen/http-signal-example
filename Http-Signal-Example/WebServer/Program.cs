using System.Collections.Concurrent;
using System.Text;
using System.Text.Json;

var builder = WebApplication.CreateBuilder(args);
var app = builder.Build();

var storage = new ConcurrentDictionary<string, List<object>>();

app.MapGet("/", () => storage);

app.MapGet("/{endpoint}", (string endpoint) => storage.ContainsKey(endpoint) ? storage[endpoint] : Enumerable.Empty<object>());

app.MapPost("/{endpoint}", async delegate (HttpContext context)
{
    string endpoint = context.Request.RouteValues["endpoint"]?.ToString() ?? "unknown";
    if (!storage.ContainsKey(endpoint))
    {
        storage.GetOrAdd(endpoint, new List<object>());
    }

    using (StreamReader reader = new(context.Request.Body, Encoding.UTF8))
    {
        storage[endpoint].Add(new
        {
            Time = DateTime.UtcNow,
            Path = context.Request.Path.Value,
            context.Request.Query,
            Body = JsonSerializer.Deserialize(await reader.ReadToEndAsync(), typeof(object))
        });

        return $"You just posted to {endpoint}";
    }
});

app.Run();
