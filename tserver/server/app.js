/**
 * Created by petrpopov on 28/04/2017.
 */
const express = require('express');
const morgan = require('morgan');
const path = require('path');
const bodyParser = require('body-parser')
const tstore = require('./storage.js');

const app = express();
app.use(bodyParser.urlencoded({ extended: false }));
app.use(bodyParser.json());
app.use(morgan(':remote-addr - :remote-user [:date[clf]] ":method :url HTTP/:http-version" :status :res[content-length] :response-time ms'));


// Serve static assets
app.use(express.static(path.resolve(__dirname, '..', 'build')));

app.post('/api/badum', function(req, res) {
    if(!req.body) return res.sendStatus(400);
    if(!req.body.chipId || !req.body.counter) return res.sendStatus(400);

    let chipId = req.body.chipId;
    let counter = req.body.counter;
    tstore.saveBadum(chipId, counter, req.ip);
    res.send('success');
});

app.post('/api/fail', function(req, res) {
    if(!req.body) return res.sendStatus(400);
    if(!req.body.chipId || !req.body.counter) return res.sendStatus(400);

    let chipId = req.body.chipId;
    let counter = req.body.counter;
    tstore.saveFail(chipId, counter, req.ip);
    res.send('success');
});

app.get('/api/chips', function(req, res) {
    res.send({chips: tstore.getChips()});
});

app.get('/api/chips/:chipId', function(req, res) {
    let chipId = req.params.chipId;
    res.send({chipId: chipId, data: tstore.getChipData(chipId)});
});

// Always return the main index.html, so react-router render the route in the client
app.get('*', (req, res) => {
    res.sendFile(path.resolve(__dirname, '..', 'build', 'index.html'));
});

module.exports = app;