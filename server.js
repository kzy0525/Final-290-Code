const express = require('express');
const { SerialPort } = require('serialport');
const { ReadlineParser } = require('@serialport/parser-readline');
const cors = require('cors');

const app = express();
const port = 5500;

app.use(cors()); // Enable Cross-Origin Resource Sharing (CORS)

// Configure the serial port (update 'COM3' or '/dev/ttyUSB0' with your Arduino's port)
const arduinoPort = new SerialPort({
    path: 'COM3', // Replace with your port name (e.g., '/dev/ttyUSB0' for Linux/Mac)
    baudRate: 9600
});

// Configure the parser for incoming serial data
const parser = arduinoPort.pipe(new ReadlineParser({ delimiter: '\n' }));

let sensorData = { temperature: '--', humidity: '--', sound: '--', tvoc: '--', ECO2: '--' };

// Read data from Arduino and parse JSON
parser.on('data', (line) => {
    try {
        sensorData = JSON.parse(line.trim());
        console.log('Received:', sensorData);
    } catch (error) {
        console.error('Error parsing JSON:', error);
    }
});

// Serve sensor data as JSON
app.get('/data', (req, res) => {
    res.json(sensorData); // Send the latest sensor data
});

// Start the server
app.listen(port, () => {
    console.log(`Server running at http://localhost:${port}`);
});
