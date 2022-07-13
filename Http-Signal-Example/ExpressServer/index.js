const express = require("express");
const bodyParser = require('body-parser');
const app = express();
app.use(bodyParser.json());

const storage = {};

app.get("/", (req, res) => res.send(storage));

app.get("/:endpoint", (req, res) => {
    const endpoint = req.params.endpoint;
    res.send(storage[endpoint] ?? []);
});

app.post("/:endpoint", (req, res) => {
    const endpoint = req.params.endpoint ?? "unknown";
    if (storage[endpoint] == undefined) {
        storage[endpoint] = [];
    }
    storage[endpoint].push({
        time: new Date().toUTCString(),
        path: req.path,
        query: req.query,
        body: req.body ?? null,
    });
    res.send(`You just posted to ${endpoint}`);
});

app.listen(5175);
console.log("Server running on port 5175");
