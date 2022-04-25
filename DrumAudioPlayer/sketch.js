var ww;


var serialConnected = false;
var port;
var ports;
//var reader;

var controllerData = {"b":0,"x":1905,"y":1925, "p":0};

var sound;

function setup() {
	progress = 0.5;
	let wh = windowHeight - 100
	ww = wh
	createCanvas(wh,wh); // make an HTML canvas element width x height pixels

	document.title = "Drummer";


	//serial = new p5.SerialPort(); // make a new instance of the serialport library
	//serial.on('list', printList); // set a callback function for the serialport list event
	
	//serial.list(); // list the serial ports

	button = createButton('Connect');
	button.position(0, 0);
	button.mousePressed(connectSerial);

	sound = new Howl({
		src: ['./drums/hat.mp3', './drums/kick.mp3', './drums/snare.mp3']
	});
	
}

function draw() {
	background(255);
	fill(0);

}


async function connectSerial(){
	port = await navigator.serial.requestPort();
	ports = await navigator.serial.getPorts();
	console.log(port);
	await port.open({ baudRate: 115200 });

	//reader = port.readable.getReader()


	const textDecoder = new TextDecoderStream();
	const readableStreamClosed = port.readable.pipeTo(textDecoder.writable);
	const reader = textDecoder.readable.getReader();
	//serialConnected = true;

	let currstr = "";

	while (true) {
		const { value, done } = await reader.read();
		if (done) {
		  // Allow the serial port to be closed later.
		  reader.releaseLock();
		  break;
		}
		// value is a Uint8Array.

		//build json string
		if(value.charAt(0) === '{') currstr = value;
		else if(value.includes('}')){
			currstr += value.substring(0, value.indexOf('}')+1);
			playBeat(currstr);
			
			//call stuff here
			currstr = "";
		}
		else{
			if (currstr.charAt(0) === '{') currstr+=value;
			else currstr = "";
		}

	}
}


function playBeat(beatStr){
	
	console.log(beatStr.charAt(1))
}


