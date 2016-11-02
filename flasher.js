var fs = require('fs-extra');
var path = require('path');

/*
 autoflasher:
 - ask for first teensy no to start with
 - ask for amount of Teensies = X
 - for i=0; i< X
 - copy template X times into folder ./output/X/*
 - edit template vars
 - teensyIP
 - universe1
 - universe2
 - mqtt tripodNo
 - mqtt publish endpoints
 - for i=0; i< X
 - wait for new teensy connected via usb
 - flash firmware

 platformio run // build
 platformio run -t upload // upload

 */

var templateDirectory = path.join(__dirname, 'template');

var outputBaseDirectory = path.join(__dirname, 'firmwares');

var firstTeensyIndex = process.argv[3];
var totalTeensies = process.argv[2];
//var totalTeensiesString = process.argv[2].toString();

console.log('Going to start flashing ', totalTeensies, ' starting from teensy number', firstTeensyIndex);

var successCount = 0;

for (var teensyIndex = 0; teensyIndex < totalTeensies; teensyIndex++) {
    var copySuccess = false, writeSuccess = false;

    var paddedIndex = pad(parseInt(teensyIndex),2);

    var firstHex = parseInt(paddedIndex.toString().split('')[0]);

    var secondHex = parseInt(paddedIndex.toString().split('')[1]);

    var replaceDefs = {
        'glow_sensortest/glow_sensortest.ino': {
            '$lastOctet': teensyIndex + 3,
            '$lastMinusOneHex': firstHex.toString(16),
            '$lastHex': secondHex.toString(16)
        },
        'glow_sensortest/Artnet.cpp': {
            '$universe1': teensyIndex*2,
            '$universe2': teensyIndex*2+1
        },
        'glow_sensortest/mqtt_ethernet.cpp': {
            '$tripodNo': teensyIndex
        }

    };

    var outputDirectory = path.join(outputBaseDirectory, teensyIndex.toString());
    try {
        fs.copySync(templateDirectory, outputDirectory);
        copySuccess = true;
        console.log('Copied the template into directory', outputDirectory);
    } catch (err) {
        console.error('Error while copying template', err);
    }

    if (copySuccess) {
        Object.keys(replaceDefs).forEach(function (defsFilename) {
            try {
                var filename = path.join(outputDirectory, defsFilename);
                console.log('Trying to read', filename);

                var fileContent = fs.readFileSync(filename).toString();
                console.log('Read the original firmware from', filename);
            } catch (err) {
                console.error('Error while reading original firmware', err);
            }

            var searchArray = Object.keys(replaceDefs[defsFilename]);

            searchArray.forEach(function(search) {
                var replace = replaceDefs[defsFilename][search];
                fileContent = fileContent.replace(search, replace);
            });

            try {
                fs.writeFileSync(path.join(outputDirectory, defsFilename), fileContent);
                writeSuccess = true;
                console.log('Copied the firmware into directory', outputDirectory);
            } catch (err) {
                console.error('Error while writing teensy specific firmware', err);
            }
        });

        successCount++;
    }
}

console.log('Successful writes', successCount);

function pad(n, width, z) {
    z = z || '0';
    n = n + '';
    return n.length >= width ? n : new Array(width - n.length + 1).join(z) + n;
}