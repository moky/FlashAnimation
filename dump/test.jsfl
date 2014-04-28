
var file = fl.configURI + 'Commands/dump.jsfl';
fl.runScript(file);

var exporter = new FlashExporter(fl.getDocumentDOM());
exporter.test();