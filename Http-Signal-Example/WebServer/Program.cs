using System.Collections.Concurrent;
using System.Text;

var builder = WebApplication.CreateBuilder(args);

var app = builder.Build();

var storage = new ConcurrentDictionary<string, List<object>>();

app.MapGet("/{endpoint}", (string endpoint) => storage.ContainsKey(endpoint) ? storage[endpoint] : Enumerable.Empty<object>());

app.MapPost("/{endpoint}", async delegate (HttpContext context)
{
    string endpoint = context.Request.RouteValues["endpoint"]?.ToString() ?? "unknown";
    if (!storage.ContainsKey(endpoint))
    {
        storage.GetOrAdd("endpoint", new List<object>());
    }

    using (StreamReader reader = new StreamReader(context.Request.Body, Encoding.UTF8))
    {
        string body = await reader.ReadToEndAsync();

        storage[endpoint].Add(new
        {
            Time = DateTime.UtcNow,
            Path = context.Request.Path,
            Query = context.Request.Query,
            Body = body
        });

        return "You just posted";
    }
});

app.Run();
