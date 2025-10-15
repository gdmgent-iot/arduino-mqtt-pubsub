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

// connectie maken
cutie.on("connect", () => {
  console.log("Verbonden met de broker");
  cutie.subscribe(topic, (error) => {
    if (error) console.error("Er ging iets mis");
    else console.log("Geabbonneerd op topic" + topic);
  });
});

// luisteren naar messages
cutie.on("message", (topic, message) => {
  $ulMessages.innerHTML += "<li>" + message + "</li>";
});

// als ik klik op de knop, moeten alle LEDs aan
$btnOn.addEventListener("click", () => {
  cutie.publish(topic, "LED ON");
});
$btnOff.addEventListener("click", () => {
  cutie.publish(topic, "LED OFF");
});
