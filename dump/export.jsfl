
var file = fl.configURI + 'Commands/dump.jsfl';
fl.runScript(file);

var exporter = new FlashExporter(fl.getDocumentDOM());
exporter.spriteSheetMetaFileName = 'Untitled_default.plist';
exporter.outputSpriteSheetIfPossible = false;
exporter.outputDataFileName = 'main.json'
exporter.run();