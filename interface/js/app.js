// dom elements
const $btnOn = document.querySelector("#on");
const $btnOff = document.querySelector("#off");
const $ulMessages = document.querySelector("#messages");

// broker data
const broker = "192.168.1.114";
const port = 9001;
const topic = "technology/arduino/demo";
const url = "ws://" + broker + ":" + port;

// mqtt client aanmaken
const cutie = mqtt.connect(url);
