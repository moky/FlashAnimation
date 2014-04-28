/*
*
* dump flash
* create by jison
* email: jisonfree@gmail.com
*/

(function() {
  // Private array of chars to use
  var CHARS = '0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz'.split('');

  Math.uuid = function (len, radix) {
    var chars = CHARS, uuid = [];
    radix = radix || chars.length;

    if (len) {
      for (var i = 0; i < len; i++) uuid[i] = chars[0 | Math.random()*radix];
    } else {
      var r;

      uuid[8] = uuid[13] = uuid[18] = uuid[23] = '-';
      uuid[14] = '4';

      for (var i = 0; i < 36; i++) {
        if (!uuid[i]) {
          r = 0 | Math.random()*16;
          uuid[i] = chars[(i == 19) ? (r & 0x3) | 0x8 : r];
        }
      }
    }

    return uuid.join('');
  };

  Math.uuidFast = function() {
    var chars = CHARS, uuid = new Array(36), rnd=0, r;
    for (var i = 0; i < 36; i++) {
      if (i==8 || i==13 ||  i==18 || i==23) {
        uuid[i] = '-';
      } else if (i==14) {
        uuid[i] = '4';
      } else {
        if (rnd <= 0x02) rnd = 0x2000000 + (Math.random()*0x1000000)|0;
        r = rnd & 0xf;
        rnd = rnd >> 4;
        uuid[i] = chars[(i == 19) ? (r & 0x3) | 0x8 : r];
      }
    }
    return uuid.join('');
  };

  Math.uuidCompact = function() {
    return 'xxxxxxxx-xxxx-4xxx-yxxx-xxxxxxxxxxxx'.replace(/[xy]/g, function(c) {
      var r = Math.random()*16|0, v = c == 'x' ? r : (r&0x3|0x8);
      return v.toString(16);
    }).toUpperCase();
  };
})();



var JSON = {};

(function () {
    'use strict';

    function f(n) {
        return n < 10 ? '0' + n : n;
    }

    if (typeof Date.prototype.toJSON !== 'function') {
        Date.prototype.toJSON = function (key) {

            return isFinite(this.valueOf()) ?
            		this.getUTCFullYear()     + '-' +
                    f(this.getUTCMonth() + 1) + '-' +
                    f(this.getUTCDate())      + 'T' +
                    f(this.getUTCHours())     + ':' +
                    f(this.getUTCMinutes())   + ':' +
                    f(this.getUTCSeconds())   + 'Z'
                : null;
        };

        String.prototype.toJSON      =
            Number.prototype.toJSON  =
            Boolean.prototype.toJSON = function (key) {
                return this.valueOf();
            };
    }

    var cx = /[\u0000\u00ad\u0600-\u0604\u070f\u17b4\u17b5\u200c-\u200f\u2028-\u202f\u2060-\u206f\ufeff\ufff0-\uffff]/g,
        escapable = /[\\\"\x00-\x1f\x7f-\x9f\u00ad\u0600-\u0604\u070f\u17b4\u17b5\u200c-\u200f\u2028-\u202f\u2060-\u206f\ufeff\ufff0-\uffff]/g,
        gap,
        indent,
        meta = {    // table of character substitutions
            '\b': '\\b',
            '\t': '\\t',
            '\n': '\\n',
            '\f': '\\f',
            '\r': '\\r',
            '"' : '\\"',
            '\\': '\\\\'
        },
        rep;

    function quote(string) {
        escapable.lastIndex = 0;
        return escapable.test(string) ? '"' + string.replace(escapable, function (a) {
            var c = meta[a];
            return typeof c === 'string'
                ? c
                : '\\u' + ('0000' + a.charCodeAt(0).toString(16)).slice(-4);
        }) + '"' : '"' + string + '"';
    }


    function str(key, holder) {

        var i,          // The loop counter.
            k,          // The member key.
            v,          // The member value.
            length,
            mind = gap,
            partial,
            value = holder[key];

        if (value && typeof value === 'object' &&
                typeof value.toJSON === 'function') {
            value = value.toJSON(key);
        }

        if (typeof rep === 'function') {
            value = rep.call(holder, key, value);
        }

        switch (typeof value) {
        case 'string':
            return quote(value);

        case 'number':
            return isFinite(value) ? String(value) : 'null';

        case 'boolean':
        case 'null':
            return String(value);

        case 'object':
            if (!value) {
                return 'null';
            }

            gap += indent;
            partial = [];
            if (Object.prototype.toString.apply(value) === '[object Array]') {
                length = value.length;
                for (i = 0; i < length; i += 1) {
                    partial[i] = str(i, value) || 'null';
                }

                v = partial.length === 0
                    ? '[]'
                    : gap
                    ? '[\n' + gap + partial.join(',\n' + gap) + '\n' + mind + ']'
                    : '[' + partial.join(',') + ']';
                gap = mind;
                return v;
            }

            if (rep && typeof rep === 'object') {
                length = rep.length;
                for (i = 0; i < length; i += 1) {
                    if (typeof rep[i] === 'string') {
                        k = rep[i];
                        v = str(k, value);
                        if (v) {
                            partial.push(quote(k) + (gap ? ': ' : ':') + v);
                        }
                    }
                }
            } else {
                for (k in value) {
                    if (Object.prototype.hasOwnProperty.call(value, k)) {
                        v = str(k, value);
                        if (v) {
                            partial.push(quote(k) + (gap ? ': ' : ':') + v);
                        }
                    }
                }
            }

            v = partial.length === 0
                ? '{}'
                : gap
                ? '{\n' + gap + partial.join(',\n' + gap) + '\n' + mind + '}'
                : '{' + partial.join(',') + '}';
            gap = mind;
            return v;
        }
    }

    if (typeof JSON.stringify !== 'function') {
        JSON.stringify = function (value, replacer, space) {
            var i;
            gap = '';
            indent = '';

            if (typeof space === 'number') {
                for (i = 0; i < space; i += 1) {
                    indent += ' ';
                }
            } else if (typeof space === 'string') {
                indent = space;
            }

            rep = replacer;
            if (replacer && typeof replacer !== 'function' &&
                    (typeof replacer !== 'object' ||
                    typeof replacer.length !== 'number')) {
                throw new Error('JSON.stringify');
            }

            return str('', {'': value});
        };
    }

    if (typeof JSON.parse !== 'function') {
        JSON.parse = function (text, reviver) {
            var j;

            function walk(holder, key) {

                var k, v, value = holder[key];
                if (value && typeof value === 'object') {
                    for (k in value) {
                        if (Object.prototype.hasOwnProperty.call(value, k)) {
                            v = walk(value, k);
                            if (v !== undefined) {
                                value[k] = v;
                            } else {
                                delete value[k];
                            }
                        }
                    }
                }
                return reviver.call(holder, key, value);
            }

            text = String(text);
            cx.lastIndex = 0;
            if (cx.test(text)) {
                text = text.replace(cx, function (a) {
                    return '\\u' +
                        ('0000' + a.charCodeAt(0).toString(16)).slice(-4);
                });
            }

            if (/^[\],:{}\s]*$/
                    .test(text.replace(/\\(?:["\\\/bfnrt]|u[0-9a-fA-F]{4})/g, '@')
                        .replace(/"[^"\\\n\r]*"|true|false|null|-?\d+(?:\.\d*)?(?:[eE][+\-]?\d+)?/g, ']')
                        .replace(/(?:^|:|,)(?:\s*\[)+/g, ''))
            ) {
                j = eval('(' + text + ')');
                return typeof reviver === 'function' ? walk({'': j}, '') : j;
            }
            throw new SyntaxError('JSON.parse');
        };
    }
}());




var CCLogger = function() {

};
(function(){
	var _level = 0;

	CCLogger.prototype.log = function(msg) {
		var tab = '  ';
		var tabList = [];
		for (var i = 0; i < _level; i ++) {
			tabList.push(tab);
		}
		var tabStr = tabList.join('');

		fl.trace(tabStr + msg);
	}

	CCLogger.prototype.warn =  function(msg) {
		alert(msg);
	}

	CCLogger.prototype.levelUp = function() {
		_level ++;
	}

	CCLogger.prototype.levelDown = function() {
		_level --;
	}
})();

//} Javascript Helper
var CCAction = function() {
	this.scaleXBy = 0.0;
	this.scaleYBy = 0.0;
	this.skewXBy = 0.0;
	this.skewYBy = 0.0
	this.rotationBy = 0.0;
	this.alphaBy = 0.0;
	this.tranformXBy = 0.0;
	this.tranformYBy = 0.0;
}

var CCSprite = function() {
	this.id = '';
	this.type = '';
	this.texture = "";

	this.firstFrame = 0;
	this.loop = '';

	this.x = 0.0;
	this.y = 0.0;

	this.matrix = null;

	this.tranformX = 0.0;
	this.tranformY = 0.0;
	
	this.scaleX = 1.0;
	this.scaleY = 1.0;
	this.skewX = 0.0;
	this.skewY = 0.0;
	this.rotation = 0.0;

	this.width = 0;
	this.height = 0;

	this.anchorX = 0.0;
	this.anchorY = 0.0;

	this.alpha = 1.0;

	this.action = null;

	this.frame = null;
};
(function(){
	

	CCSprite.prototype.initWithElement = function(element) {
		
	}
})();


var CCFrame = function(flashFrame) {
	this.type = flashFrame.tweenType;
	this.startFrame = flashFrame.startFrame;
	// this.nextFrame = flashFrame.startFrame + flashFrame.duration;
	this.duration = flashFrame.duration;
	this.flashFrame = flashFrame;
	this.sprites = [];

	this.layer = null;
};
(function(){
	CCFrame.prototype.getPath = function() {
		return [this.layer.symbol.name, this.layer.name, this.startFrame].join('/');
	}

	CCFrame.prototype.addSprite = function(sprite) {
		this.sprites.push(sprite);
	}

	CCFrame.prototype.addSpriteAtHead = function(sprite) {
		this.sprites.unshift(sprite);
	}
})();


var CCLayer = function(flashLayer) {
	this.name = flashLayer.name;
	this.type = flashLayer.layerType;
	this.parentLayerName = flashLayer.parentLayer ? flashLayer.parentLayer.name : null;
	this.frameCount = flashLayer.frameCount;
	this.frames = flashLayer.frames;
	this.index = 0;
	this.keyFrames = [];
	this.symbol = null;
};
(function(){
	CCLayer.prototype.addKeyFrame = function(ccFrame) {
		this.keyFrames.push(ccFrame);
		ccFrame.layer = this;
	}
})();


var CCSymbol = function(item) {
	this.name = item.name;
	this.minScale = -1.0;
	this.maxScale = -1.0;
	this.type = item.itemType;
	this.isVisited = false;
	this.item = item;
	this.width = 0.0;
	this.height = 0.0;
	this.offsetX = 0.0;
	this.offsetY = 0.0;

	this.left = Number.MAX_VALUE;
	this.top = Number.MAX_VALUE;
	this.right = -Number.MAX_VALUE;
	this.bottom = -Number.MAX_VALUE;

	this.isStatic = false;
	this.frameCount = 0;
	this.texture = '';
	this.layers = [];

	this.subSymbols = [];

	this.scalesInSuperSymbol = {};
};

(function(){
	CCSymbol.prototype.updateMinScaleIfNeedWithScale = function(scale) {
		if (this.minScale < 0) {
			this.minScale = scale;
		} else {
			if (scale < this.minScale) {
				this.minScale = scale;
			}
		}
	}

	CCSymbol.prototype.updateMaxScaleIfNeedWithScale = function(scale) {
		if (this.maxScale < 0) {
			this.maxScale = scale;
		} else {
			if (scale > this.maxScale) {
				this.maxScale = scale;
			}
		}
	}

	CCSymbol.prototype.updateScaleIfNeedWithScale = function(scale) {
		this.updateMaxScaleIfNeedWithScale(scale);
		this.updateMinScaleIfNeedWithScale(scale);
	}

	CCSymbol.prototype.updateScaleIfNeedWithSuperSymbol = function(scale, superSymbol) {
		var scaleInSymbol = this.scalesInSuperSymbol[superSymbol.name];
		if (scaleInSymbol == null) {
			this.scalesInSuperSymbol[superSymbol.name] = {'min':scale, 'max':scale};
			this.updateMaxScaleIfNeedWithScale(scale * superSymbol.getMaxScale());
			this.updateMinScaleIfNeedWithScale(scale * superSymbol.getMinScale());
		} else {
			var minScale = scaleInSymbol['min'];
			var maxScale = scaleInSymbol['max'];
			if (scale < minScale) {
				minScale = scale;
		}
			if (scale > maxScale) {
				maxScale = scale;
			}
			this.scalesInSuperSymbol[superSymbol.name] = {'min':minScale, 'max':maxScale};
			this.updateMaxScaleIfNeedWithScale(minScale * superSymbol.getMaxScale());
			this.updateMinScaleIfNeedWithScale(maxScale * superSymbol.getMinScale());
		}

		this.subSymbols.forEach(function(subSymbol){
			subSymbol.updateScaleIfNeedWithSuperSymbol(subSymbol.getMaxScale(), this);
		}, this);
	}

	CCSymbol.prototype.getMinScale = function() {
		if (this.minScale < 0) {
			return 1.0;
		} else {
			return this.minScale;
		}
	}

	CCSymbol.prototype.getMaxScale = function() {
		if (this.maxScale < 0) {
			return 1.0;
		} else {
			return this.maxScale;
		}
	}

	CCSymbol.prototype.updateSizeIfNeedWithRect = function(rect) {
		if (rect.left < this.left) {
			this.left = rect.left;
		}
		if (rect.top < this.top) {
			this.top = rect.top;
		}
		if (rect.right > this.right) {
			this.right = rect.right;
		}
		if (rect.bottom > this.bottom) {
			this.bottom = rect.bottom;
		}
	}

	CCSymbol.prototype.addLayer = function(layer) {
		layer.symbol = this;
		this.layers.push(layer);
	}

	CCSymbol.prototype.addSubSymbolIfNeed = function(subSymbol) {
		if (this.subSymbols.indexOf(subSymbol) == -1) {
			this.subSymbols.push(subSymbol);
		}
	}
})();

var CCSymbolLibrary = function() {
	this.symbols = [];
};

(function(){
	CCSymbolLibrary.prototype.addSymbol = function(symbol) {
		this.symbols.push(symbol);
	};

	CCSymbolLibrary.prototype.getSymbolWithName = function(symbolName) {
		for (var i = 0; i < this.symbols.length; i ++) {
			if (this.symbols[i].name === symbolName) {
				return this.symbols[i];
			}
		}

		return null;
	};

	CCSymbolLibrary.prototype.getSymbolWithItem = function(libraryItem) {
		return this.getSymbolWithName(libraryItem.name);
	}

	CCSymbolLibrary.prototype.getSymbolWithItemCreateIfNoExist = function(libraryItem) {
		var symbol = this.getSymbolWithName(libraryItem.name);
		if (symbol === null) {
			symbol = new CCSymbol(libraryItem);
			this.addSymbol(symbol);
		}

		return symbol;
	}
})();




//FlashExporter Class {
var FlashExporter = function(document) {
	if (!document) {
		alert('document is null');
		return;
	}
	this.document = document;
	var docName = document.name;
	this.imagePrefix = docName.substr(0, docName.lastIndexOf('.')) + '-' + Math.uuid(5, 62);
	this.symbolLibrary = new CCSymbolLibrary();
	this.symbolFolderName = '-----CCTempSymbol-----';
	this.spriteSheetMetaFileName = 'spritesheet.plist';
	this.outputDataFileName = 'main.json';
	this.outputSpriteSheetIfPossible = false;
	this.dataFormatter = new Cocos2dDataFormatter();
};

(function (){//implement of FlashExporter
	var _logger = new CCLogger();

	var log = function(msg) {
		_logger.log(msg);
	};

	var logUp = function(msg) {
		_logger.log('<' + msg + '>');
		_logger.levelUp();
	};

	var logDown = function(msg) {
		_logger.levelDown();
		_logger.log('</' + msg + '>');
	};

	var warn = function(msg) {
		_logger.log(msg);
	};

	var duplicateString = function(str, multiplier) {
		var strList = [];
		for (var i =0; i < multiplier; i++) {
			strList.push(str);
		}

		return strList.join('');
	};

	var _markLevel = 0;
	var markWithDumpFunc = function(mark, dumpFunc/*, thisObj*/) {
		if (typeof dumpFunc != "function") {
      		throw new TypeError('dumpFunc must be a function');
      	}

		var tabStr = duplicateString('  ', _markLevel);
		log(tab + '<' + mark + '>');
		_markLevel ++;

		var thisObj = arguments[2];
		dumpFunc.call(thisObj);

		_markLevel --;
		log(tab + '</' + mark + '>');
	};

	//private method {
	var generateUniqueId = (function() {
		var id = 0;
		var salt = 'qfvkvu'
		return function() {
			id++;
			return 'SS' + '-' + salt + '-' + id;
		}
	})();

	var generateImageName = (function(){
		var id = 0;
		return function(prefix) {
			id++;
			var time = new Date().getTime();
			var part1 = id.toString(10);
			var part2 = time.toString(16);
			return prefix + '-' + part1;
		}
	})();

	var roundToTwip = function(value) {
		return Math.round(value*20) / 20;
	};

	var roundToTwip2 = function(value) {
		return Math.round(value * 200) / 200;
	};

	var skewsAreEqual = function(skewX, skewY) {
		var SKEW_EQUALITY_DELTA = .1;
		return ((Math.abs(skewY - skewX) % 360) <= SKEW_EQUALITY_DELTA);
	};

	var getMatrixScaleX = function(matrix) {
		var angleYRad = Math.atan2(matrix.b, matrix.a);
		var cos = Math.cos(angleYRad);
		var scaleX = matrix.a / cos;
		if (Math.abs(cos) < 1e-12)
			scaleX = matrix.b / Math.sin(angleYRad);
		var scaleRound = 1000;
		scaleX = Math.round(scaleX*scaleRound)/scaleRound;
		return scaleX;
	};

	var getMatrixScaleY = function(matrix) {
		var angleXRad = Math.atan2(-matrix.c, matrix.d);
		var cos = Math.cos(angleXRad);
		var scaleY = matrix.d / cos;
		if (Math.abs(cos) < 1e-12)
			scaleY = -matrix.c / Math.sin(angleXRad);
		var scaleRound = 1000;
		scaleY = Math.round(scaleY*scaleRound)/scaleRound;
		return scaleY;
	};


	 var getMatrixSkewX = function(matrix) {
		var angleXRad = Math.atan2(-matrix.c, matrix.d);
		var angleRound = 10;
		var angleX = Math.round(angleXRad * 180/Math.PI * angleRound)/angleRound;
		return angleX;
	};

	var getMatrixSkewY = function(matrix) {
		var angleYRad = Math.atan2(matrix.b, matrix.a);
		var angleRound = 10;
		var angleY = Math.round(angleYRad * 180/Math.PI * angleRound)/angleRound;
		return angleY;
	}

	function getScaleX(m) {
		return Math.sqrt(m.a*m.a + m.b*m.b);
	}
	function setScaleX(m, scaleX) {
		var oldValue = getScaleX(m);
		// avoid division by zero
		if (oldValue) {
			var ratio = scaleX / oldValue;
			m.a *= ratio;
			m.b *= ratio;
		} else {
			var skewYRad = getSkewYRadians(m);
			m.a = Math.cos(skewYRad) * scaleX;
			m.b = Math.sin(skewYRad) * scaleX;
		}
	}

	function getScaleY(m) {
		return Math.sqrt(m.c*m.c + m.d*m.d);
	}
	function setScaleY(m, scaleY) {
		var oldValue = getScaleY(m);
		// avoid division by zero
		if (oldValue) {
			var ratio = scaleY / oldValue;
			m.c *= ratio;
			m.d *= ratio;
		} else {
			var skewXRad = getSkewXRadians(m);
			m.c = -Math.sin(skewXRad) * scaleY;
			m.d =  Math.cos(skewXRad) * scaleY;
		}
	}

	function getSkewXRadians(m) {
		return Math.atan2(-m.c, m.d);
	}
	function setSkewXRadians(m, skewX) {
		var scaleY = getScaleY(m);
		m.c = -scaleY * Math.sin(skewX);
		m.d =  scaleY * Math.cos(skewX);
	}

	function getSkewYRadians(m) {
		return Math.atan2(m.b, m.a);
	}
	function setSkewYRadians(m, skewY) {
		var scaleX = getScaleX(m);
		m.a = scaleX * Math.cos(skewY);
		m.b = scaleX * Math.sin(skewY);
	}

	function getSkewX(m) {
		return Math.atan2(-m.c, m.d) * (180/Math.PI);
	}
	function setSkewX(m, skewX) {
		setSkewXRadians(m, skewX*(Math.PI/180));
	}

	function getSkewY(m) {
		return Math.atan2(m.b, m.a) * (180/Math.PI);
	}
	function setSkewY(m, skewY) {
		setSkewYRadians(m, skewY*(Math.PI/180));
	}

	function getRotationRadians(m) {
		var skewEqualityDelta = .0001; // customizable
		var skewXRad = getSkewXRadians(m);
		var skewYRad = getSkewYRadians(m);
		if (Math.abs(skewXRad-skewYRad) <= skewEqualityDelta)
			return skewYRad;
		return NaN;
	}
	function setRotationRadians(m, rotation) {
		setSkewXRadians(m, rotation);
		setSkewYRadians(m, rotation);
	}

	function getRotation(m) {
		return getRotationRadians(m)*(180/Math.PI);
	}
	function setRotation(m, rotation) {
		setSkewXRadians(m, rotation*(Math.PI/180));
		setSkewYRadians(m, rotation*(Math.PI/180));
	}
	
	var getFlashVersionNumber = function() {
		var verStr = fl.version;
		var verNumStr = verStr.substr(verStr.indexOf(' ') + 1, verStr.length - 1);
		var verNum = verNumStr.substr(0, verNumStr.indexOf(','));
		
		return parseInt(verNum, 10);
	};

	var checkDocumentIfCanExport = function(doc) {
		var versionNum = getFlashVersionNumber();
		if (versionNum < 12) {
			return '必须使用CS6或以上的flash来导出';
		}


		var docTimelines = doc.timelines;

		if (docTimelines.length != 1) {
			return '文档中有且只能有一个场景';
		}

		var sceneTimeline = docTimelines[0];
		if (sceneTimeline.layerCount != 1) {
			return '场景下只能有且只有一个层';
		}

		var sceneLayer = sceneTimeline.layers[0];
		if (sceneLayer.frameCount != 1) {
			return '场景下只能有且只有一个桢';
		}

		var sceneFrame = sceneLayer.frames[0];
		if (sceneFrame.elements.length != 1) {
			return '场景下只能有且只有一个元件';
		}

		var sceneElement = sceneFrame.elements[0];
		if (sceneElement.elementType != 'instance') {
			return '场景下只能有且只有一个元件'
		}

		return null;
	};

	var findTheRootSymbol = function(doc) {
		var rootFrame = doc.timelines[0].layers[0].frames[0];

		var rootFrameFirstElement = rootFrame.elements[0];
		if (!rootFrameFirstElement || rootFrameFirstElement.elementType != 'instance' || rootFrameFirstElement.instanceType != 'symbol') {
			return null;
		} else {
			rootFrameFirstElement.selected = true;
			return rootFrameFirstElement.libraryItem;
		}
	};

	var findTheRootElement = function(doc) {
		return doc.timelines[0].layers[0].frames[0].elements[0];
	};

	var isElementsAllShape = function(elements) {
		for (var elementIndex = 0, elementCount = elements.length; elementIndex < elementCount; elementIndex ++) {
			element = elements[elementIndex];
			if (element.elementType != 'shape') {
				return false;
			}

			if (element.isGroup) {
				if (!isElementsAllShape(element.members)) {
					return false;
				}
			}
		}

		return true;
	};

	var editInSymbol = function(symbol, editFunc /*, thisObj */) {
		if (typeof editFunc != "function") {
      		throw new TypeError('editFunc must be a function');
      	}

      	var thisObj = arguments[2];

		var doc = thisObj ? thisObj.document : fl.getDocumentDOM();
		// var currentTimeline = doc.getTimeline();
		// var currentLayer = currentTimeline.currentLayer;
		// var currentFrame = currentTimeline.currentFrame;
		// var needBack = false;
		// if (currentTimeline.name != symbol.name) {
		// 	doc.library.editItem(symbol.name);
		// 	needBack = true;
		// }

		doc.enterEditMode('inPlace');

		var result = editFunc.call(thisObj);

		doc.exitEditMode();

		// if (needBack) {
		// 	doc.library.editItem(currentTimeline.name);
		// 	currentTimeline.setSelectedLayers(currentLayer, currentLayer);
		// 	currentTimeline.setSelectedFrames(currentFrame, currentFrame);
		// }

		return result;
	};

	var isSymbolStatic = function(doc, symbol) {
		
		// doc.library.selectItem(symbol.name);
		

		return editInSymbol(symbol, function () {
			var timeline = doc.getTimeline();
			if ( timeline.frameCount != 1 ) {
				return false;
			}

			var isStatic = true;
			for (var layerIndex = 0; layerIndex < timeline.layerCount; layerIndex ++) {
				var layer = timeline.layers[layerIndex];
				var frame = layer.frames[0];

				if (!isElementsAllShape(frame.elements)) {
					isStatic = false;
					break;
				}
			}
			return isStatic;
		});
	};

	var isCurrentSymbolStatic = function(doc) {
		var timeline = doc.getTimeline();
		if ( timeline.frameCount != 1 ) {
			return false;
		}

		var isStatic = true;
		for (var layerIndex = 0; layerIndex < timeline.layerCount; layerIndex ++) {
			var layer = timeline.layers[layerIndex];
			var frame = layer.frames[0];

			if (!isElementsAllShape(frame.elements)) {
				isStatic = false;
				break;
			}
		}
		return isStatic;
	};

	var selectElement = function(element, doc) {
		var allSelection = doc.selection;
		allSelection.forEach(function(item){
			item.selected = false;
		});
		element.selected = true;
		
		if (doc.selection.length != 1) {

			var centerX = (element.left + element.right) / 2.0;
			var centerY = (element.top + element.bottom) / 2.0;

			doc.mouseClick({x:centerX, y:centerY}, false, false);

			doc.selection.forEach(function(item){
				item.selected = false;
			});

			element.selected = true;

			if (doc.selection.length != 1) {
				throw new Error('can not select specify element');
			}	
		}
	};
	//}private method

	FlashExporter.prototype.test = function() {
		fl.outputPanel.clear();

		var doc = fl.getDocumentDOM();
		var exporter = this;
		
		var testTransformationPoint = function() {
			var point  = doc.getTransformationPoint();
			fl.trace('(' + point.x + ',' + point.y + ')');
		}

		var testTraceElement = function() {
			if (doc.selection.length == 1) {
				var selection = doc.selection[0];
				exporter.traceElement(selection);
			}
		}

		var testTraceFrame = function() {
		 	var timeline = doc.getTimeline();
			var currentFrame = timeline.layers[timeline.currentLayer].frames[timeline.currentFrame];
			exporter.traceFrame(currentFrame);

			currentFrame.elements.forEach(function(element){
				exporter.traceElement(element);
			});
		}
		
		var testSelect = function() {
			// var timeline = doc.getTimeline();

			// var layerIndex = 3;
			// var frameIndex = 130;
			// timeline.setSelectedLayers(layerIndex, layerIndex);
			// timeline.setSelectedFrames(frameIndex, frameIndex);
			// var currentFrame = timeline.layers[layerIndex].frames[frameIndex];

			// // timeline.layers.forEach(function(layer, index){
			// // 	if (index != layerIndex) {
			// // 		layer.locked = true;
			// // 	}
			// // });

			// var elements = currentFrame.elements;
			// var element = elements[0];
			// element.locked = false;
			// element.selected = false;

			// // doc.selectAll();
			// // doc.selectNone();

			// log('left:' + element.left + ', top:' + element.top + ', width:' + element.width + ', height:' + element.height);

			// // doc.unlockAllElements();
			// var selection = doc.selection;
			// log('selection count:' + selection.length);


			// selectElement(element, doc);

			// // timeline.layers.forEach(function(layer, index){
			// // 	if (index != layerIndex) {
			// // 		layer.locked = false;
			// // 	}
			// // });

			var rect = doc.getSelectionRect();
			for (var key in rect) {
				log(key + ':' + rect[key]);
			}
			//doc.deleteEnvelope();
			// selectElement(element, doc);
			log('selection count:' + doc.selection.length);
		}

		var isSelectionHasGroup = function() {
			var hasGroup = false;
			var selection = doc.selection;
			for (var i = 0; i < selection.length; i ++) {
				if (selection[i].isGroup) {
					hasGroup = true;
					break;
				}
			}

			return hasGroup;
		};

		var testConvertToBitmap = function() {
			var name = "abcdefghijlmn";

			var tmpSymbolName = '-----TempSymbolForBitmap----';

			doc.convertToSymbol("graphic", tmpSymbolName, "center");

			var shape = doc.selection[0];
			var p = {'x':shape.transformX, 'y':shape.transformY};

			doc.exportInstanceToLibrary(1, name);

			doc.library.addItemToDocument(p, name);

			testTraceElement();

			doc.deleteSelection();

			doc.selectAll();
			doc.breakApart();

			doc.library.deleteItem(tmpSymbolName);
		}
		
		var testEditSymbol = function() {
			editInSymbol(null, function(){
				alert('here');
			}, this);
		}

		var testElementDepth = function() {
			if (doc.selection.length != 1) {
				return;
			}

			var element = doc.selection[0];
			fl.trace('depth: ' + element.depth);
		}

		var testLibraryEdit = function() {
			var itemPath = '编程/编程/编程/大熊猫';
			doc.library.editItem(itemPath);
		}

		var testLibraryCreateFolder = function() {
			doc.library.addNewItem('graphic', 'a/b/c');
		}

		var testLibraryRename = function() {
			var library = doc.library;
			var itemName = 'a/b/c';
			library.addNewItem('graphic', itemName);
			library.selectItem(itemName);

			library.addNewItem('folder', 'e/f');
			library.moveToFolder('e/f', itemName);
		}

		var testLibrarySelect = function() {
			var library = doc.library;

			library.selectItem(this.symbolFolderName + '/' + '导出类');
			var items = library.getSelectedItems();

			items.forEach(function(item){
				fl.trace(item.name);
			});
		}

		testTraceElement();
	}

	FlashExporter.prototype.run = function() {
		var doc = this.document;
		var errorStr = checkDocumentIfCanExport(doc);
		if (errorStr) {
			log(errorStr);
			throw new Error(errorStr);
		}

		fl.showIdleMessage(false);
		fl.outputPanel.clear();
		this.prepareOutputFolder();

		if (doc.library.itemExists(this.symbolFolderName)) {
			doc.library.deleteItem(this.symbolFolderName);
		}

		doc.library.newFolder(this.symbolFolderName);
		
		doc.editScene(0);
		var rootElement = findTheRootElement(doc);

		if (!rootElement || rootElement.elementType != 'instance' || rootElement.instanceType != 'symbol') {
			warn('场景中只能有一个元件实例');
			return;
		}
		var rootSymbol = this.symbolLibrary.getSymbolWithItemCreateIfNoExist(rootElement.libraryItem);

		{//handle symbol scale
			var scaleToScene = rootElement.scaleX > rootElement.scaleY ? rootElement.scaleX : rootElement.scaleY;
			rootSymbol.updateScaleIfNeedWithScale(roundToTwip2(scaleToScene));
		}
		selectElement(rootElement, doc);
		this.dumpSymbol(rootSymbol);

		this.exportImageWithMaxScale();
		// this.exportImage();
		doc.library.deleteItem(this.symbolFolderName);

		var dataString = this.exportData();
		var dataOutputPath = this.getDataOutputPath();
		FLfile.write(dataOutputPath, dataString);

		doc.revert();
		alert('done');
	}

	{
		var _outputFolderPath = null;
		FlashExporter.prototype.getOutputFolderPath = function() {
			if (!_outputFolderPath) {
				var docUri = this.document.pathURI;
				var docName = this.document.name;
				_outputFolderPath = docUri.substr(0, docUri.lastIndexOf('/')) + "/dump/" + docName.substr(0, docName.lastIndexOf('.'));
			}
			
			return _outputFolderPath;
		}

		var _imageOutputFolderPath = null;
		FlashExporter.prototype.getImageOutputFolderPath = function() {
			if (!_imageOutputFolderPath) {
				var outputFolderPath = this.getOutputFolderPath();
				_imageOutputFolderPath = outputFolderPath + '/images';
			}
			
			
			return _imageOutputFolderPath;
		}

		FlashExporter.prototype.getImageOutputPath = function(name) {
			return this.getImageOutputFolderPath() + '/' + name;
		}

		FlashExporter.prototype.getDataOutputPath = function() {
			return this.getOutputFolderPath() + '/' + this.outputDataFileName;
		}

		FlashExporter.prototype.prepareOutputFolder = function() {
			var outputFolderPath = this.getOutputFolderPath();
			var imageOutputFolderPath = this.getImageOutputFolderPath();
			FLfile.remove(imageOutputFolderPath);
			FLfile.remove(outputFolderPath);
			FLfile.createFolder(outputFolderPath);
			FLfile.createFolder(imageOutputFolderPath);
		}
	}

	FlashExporter.prototype.dumpSymbol_backup = function(symbol) {
		if (symbol.isVisited) {
			return;
		}

		logUp('symbol name="' + symbol.name + '"');
		var doc = this.document;


		var setupSymbolGeometry = function(symbol) {
			doc.selectAll();
			var rect = doc.getSelectionRect();
			if (!rect) {
				throw new Error('can not select anything');
			}
			doc.selectNone();

			var width = rect.right - rect.left;
			var height = rect.bottom - rect.top;

			symbol.left = rect.left;
			symbol.top = rect.top;
			symbol.right = rect.right;
			symbol.bottom = rect.bottom;

			symbol.width = Math.floor(width);
			if (symbol.width <= 0) {
				symbol.width = 1.0;
			}
			symbol.height = Math.floor(height);
			if (symbol.height <= 0) {
				symbol.height = 1.0;
			}
			symbol.offsetX = - rect.left;
			symbol.offsetY = - rect.top;
		}

		if (isSymbolStatic(this.document, symbol)) {
			log('static symbol');
			var imageName = generateImageName(this.imagePrefix);
			var imagePath = this.getImageOutputPath(imagePath);
			// symbol.item.exportToPNGSequence(imagePath);

			editInSymbol(symbol, function() {
				var timeline = doc.getTimeline();
				timeline.layers.forEach(function(layer){
					layer.locked = false;
				});

				doc.selectAll();
				if (doc.selection.length == 0) {
					symbol.isStatic = true;
					symbol.texture = '';
					symbol.frameCount = 1;
					symbol.width = 1.0;
					symbol.height = 1.0;
					return;
				}

				setupSymbolGeometry(symbol);

				symbol.isStatic = true;
				symbol.texture = imageName;
				symbol.frameCount = 1;
			});
		} else {

			editInSymbol(symbol, function() {
				// setupSymbolGeometry(symbol);
				
				var timeline = doc.getTimeline();
				symbol.frameCount = timeline.frameCount;
				var layers = timeline.layers;
				for (var layerIndex = layers.length - 1; layerIndex >= 0; layerIndex --) {
					var flashLayer = layers[layerIndex];
					if (flashLayer.layerType == 'guide') {
						continue;
					}

					var firstFrame = flashLayer.frames[0];
					var layerIsEmpty = flashLayer.frameCount == 0 || ( firstFrame.elements.length == 0 && firstFrame.duration == flashLayer.frameCount );

					if (!layerIsEmpty) {
						flashLayer.locked = false;
						flashLayer.visible = true;
						var layer = new CCLayer(flashLayer);
						symbol.addLayer(layer);
						layer.index = layerIndex;
						timeline.setSelectedLayers(layerIndex, layerIndex);
						this.dumpLayer(layer, symbol);
					}
				}

				symbol.width = Math.floor(symbol.right - symbol.left);
				if (symbol.width <= 0) {
					symbol.width = 1.0;;
				}
				symbol.height = Math.floor(symbol.bottom - symbol.top);
				if (symbol.height <= 0) {
					symbol.height = 1.0;
				}
				symbol.offsetX = - symbol.left;
				symbol.offsetY = - symbol.top;

			}, this);
		}

		log('subSymbols:' + symbol.subSymbols.length);
		symbol.subSymbols.forEach(function(subSymbol){
			if (subSymbol.isVisited == true) {
				return;
			}


		}, this);

		symbol.isVisited = true;
		logDown('symbol');
	}

	FlashExporter.prototype.dumpSymbol = function(symbol) {
		if (symbol.isVisited) {
			return;
		}

		logUp('symbol name="' + symbol.name + '"');
		var doc = this.document;

		doc.library.editItem(symbol.name);

		if (isCurrentSymbolStatic(doc)) {
			this.dumpStaticSymbol(symbol);
		} else {
			this.dumpDynamicSymbol(symbol);
		}

		symbol.subSymbols.forEach(function(subSymbol){
			if (subSymbol.isVisited == true) {
				return;
			}

			this.dumpSymbol(subSymbol);
		}, this);

		symbol.isVisited = true;
		logDown('symbol');
	}

	FlashExporter.prototype.dumpStaticSymbol = function(symbol) {
		var doc = this.document;

		log('static symbol');

		var timeline = doc.getTimeline();
		timeline.layers.forEach(function(layer){
			layer.locked = false;
		});

		doc.selectAll();
		if (doc.selection.length == 0) {
			symbol.isStatic = true;
			symbol.texture = '';
			symbol.frameCount = 1;
			symbol.width = 1.0;
			symbol.height = 1.0;
			return;
		}

		var imageName = generateImageName(this.imagePrefix);
		{
			var library = doc.library;
			var tmpSymbolName = '-----TempSymbolForBitmap----';

			doc.convertToSymbol("graphic", tmpSymbolName, "center");
			var shapeGraphic = doc.selection[0];
			var transformPoint = {'x':shapeGraphic.transformX, 'y':shapeGraphic.transformY};

			doc.exportInstanceToLibrary(1, imageName);

			library.addItemToDocument(transformPoint, imageName);
			var bitmap = doc.selection[0];

			symbol.left = bitmap.left;
			symbol.top = bitmap.top;
			symbol.right = bitmap.right;
			symbol.bottom = bitmap.bottom;
			symbol.width = bitmap.width;
			symbol.height = bitmap.height;
			symbol.offsetX = -symbol.left;
			symbol.offsetY = -symbol.top;

			doc.deleteSelection();

			doc.selectAll();
			doc.breakApart();

			library.deleteItem(imageName);
			library.deleteItem(tmpSymbolName);
		}

		// var rect = doc.getSelectionRect();
		// if (!rect) {
		// 	throw new Error('can not select anything');
		// }
		// doc.selectNone();

		// var width = rect.right - rect.left;
		// var height = rect.bottom - rect.top;

		// symbol.left = rect.left;
		// symbol.top = rect.top;
		// symbol.right = rect.right;
		// symbol.bottom = rect.bottom;

		// symbol.width = Math.floor(width);
		// if (symbol.width <= 0) {
		// 	symbol.width = 1.0;
		// }
		// symbol.height = Math.floor(height);
		// if (symbol.height <= 0) {
		// 	symbol.height = 1.0;
		// }
		// symbol.offsetX = - rect.left;
		// symbol.offsetY = - rect.top;

		symbol.isStatic = true;
		symbol.texture = imageName;
		symbol.frameCount = 1;		
	}

	FlashExporter.prototype.dumpDynamicSymbol = function(symbol) {
		var doc = this.document;

		var timeline = doc.getTimeline();
		symbol.frameCount = timeline.frameCount;
		var layers = timeline.layers;
		for (var layerIndex = layers.length - 1; layerIndex >= 0; layerIndex --) {
			var flashLayer = layers[layerIndex];
			if (flashLayer.layerType == 'guide') {
				continue;
			}

			var firstFrame = flashLayer.frames[0];
			var layerIsEmpty = flashLayer.frameCount == 0 || ( firstFrame.elements.length == 0 && firstFrame.duration == flashLayer.frameCount );

			if (!layerIsEmpty) {
				flashLayer.locked = false;
				flashLayer.visible = true;
				var layer = new CCLayer(flashLayer);
				symbol.addLayer(layer);
				layer.index = layerIndex;
				timeline.setSelectedLayers(layerIndex, layerIndex);
				this.dumpLayer(layer, symbol);
			}
		}

		symbol.width = Math.floor(symbol.right - symbol.left);
		if (symbol.width <= 0) {
			symbol.width = 1.0;;
		}
		symbol.height = Math.floor(symbol.bottom - symbol.top);
		if (symbol.height <= 0) {
			symbol.height = 1.0;
		}
		symbol.offsetX = - symbol.left;
		symbol.offsetY = - symbol.top;
	}

	FlashExporter.prototype.dumpLayer = function(layer, symbol) {
		logUp('layer name="' + layer.name + '"');
		var frames = layer.frames;
		layer.frames = null;
		for (var frameIndex = 0; frameIndex < frames.length; frameIndex ++) {
			var flashFrame = frames[frameIndex];
			if (flashFrame.startFrame == frameIndex) {
				var ccFrame = new CCFrame(flashFrame);
				layer.addKeyFrame(ccFrame);
			}
		}
		
		layer.keyFrames.forEach(function(frame){
			var frameIndex = parseInt(frame.startFrame, 10);
			this.document.getTimeline().setSelectedFrames(frameIndex, frameIndex);
			this.dumpFrame(frame, layer, symbol);
		}, this);

		layer.keyFrames.forEach(function(frame, keyFrameIndex){
			if (frame.type == 'motion') {
				this.tweenKeyFrame(frame, keyFrameIndex, layer);
			} else if (frame.type == 'none') {
				;
			} else {
				warn('暂时不支持形状补间:' + frame.getPath());
				//todo
				;
			}
		}, this);

		logDown('layer');
	}

	FlashExporter.prototype.tweenKeyFrame = function(frame, keyFrameIndex, layer) {
		if (frame.duration == 1) {
			return;
		}

		if (layer.keyFrames.length <= keyFrameIndex + 1) {
			warn('补间动画缺少关键帧:' + frame.getPath());
			return;
		}
		var nextKeyFrame = layer.keyFrames[keyFrameIndex + 1];

		log('tween:' + frame.startFrame);
		if (frame.sprites.length != 1 || nextKeyFrame.sprites.length != 1) {
			warn('补间动画缺少精灵');
			return;
		}

		var sourceSprite = frame.sprites[0];
		var targetSprite = nextKeyFrame.sprites[0];

		if (sourceSprite.id != targetSprite.id) {
			warn('补间动画中的精灵不一致:' + frame.getPath());
			return;
		}

		var action = new CCAction();

		var actionKeys = ['transformX', 'transformY', 'skewX', 'skewY', 'scaleX', 'scaleY', 'rotation', 'alpha'];
		var actionIsEmpty = true;
		actionKeys.forEach(function(actionKey){
			if (targetSprite[actionKey] != sourceSprite[actionKey]) {
				actionIsEmpty = false;
				action[actionKey + 'By'] = roundToTwip(targetSprite[actionKey] - sourceSprite[actionKey]);
			}
		});

		if (!actionIsEmpty) {
			sourceSprite.action = action;
		}
	}

	FlashExporter.prototype.dumpFrame = function(frame, layer, symbol) {
		logUp('frame index="' + frame.startFrame + '"');
		var flashFrame = frame.flashFrame;

		for (var elementIndex = flashFrame.elements.length - 1; elementIndex >= 0; elementIndex --) {
		 	this.dumpFlashElement(flashFrame.elements[elementIndex], frame);
		}

		var doc = this.document;
		doc.selectAll();
		var rect = doc.getSelectionRect();
		symbol.updateSizeIfNeedWithRect(rect);

		logDown('frame');
	}

	FlashExporter.prototype.dumpFlashElement = function(flashElement, frame) {
		if (flashElement.elementType == 'shape') {
			this.dumpFlashShape(flashElement, frame);
		} else if (flashElement.elementType == 'instance') {
			this.dumpFlashInstance(flashElement, frame);
		} else {
			var elementPath = this.getElementPath(flashElement, frame);
			warn('暂不支持指定的元素类型:' + flashElement.elementType + ' (' + elementPath + ')');
		}
	}

	FlashExporter.prototype.createSpriteWithElement = function(element, frame) {
		var sprite = new CCSprite();

		sprite.type = element.elementType;
		if (element.elementType == 'instance') {
			if (element.instanceType == 'symbol') {
				if (element.symbolType == 'graphic') {
					sprite.type = 'graphic';
					sprite.firstFrame = element.firstFrame;
					sprite.loop = element.loop;
				} else {
					sprite.type = 'movie clip';
				}
				
				sprite.id = element.libraryItem.name;
				sprite.alpha = element.colorAlphaPercent / 100.0;
			} else if (element.instanceType == 'bitmap') {
				sprite.type = 'shape';

				var bitmapSymbol = this.symbolLibrary.getSymbolWithName(element.libraryItem.name);
				if (bitmapSymbol) {
					sprite.id = element.libraryItem.name;
				} else {
					sprite.id = generateUniqueId();
				}
				
				sprite.texture = element.libraryItem.name;
			} else {
				throw new TypeError('unsupport instance type:' + element.instanceType);
			}
			
		} else if (element.elementType == 'shape') {
			sprite.type = 'shape';
			sprite.id = generateUniqueId();
			sprite.texture = generateImageName(this.imagePrefix);
		} else {
			throw new TypeError('unsupport element type:' + element.elementType);
		}

		sprite.x = roundToTwip(element.x);
		sprite.y = roundToTwip(element.y);

		sprite.transformX = roundToTwip(element.transformX);
		sprite.transformY = roundToTwip(element.transformY);

		var matrix = element.matrix;
		sprite.matrix = matrix;
		sprite.scaleX = getMatrixScaleX(matrix);
		sprite.scaleY = getMatrixScaleY(matrix);

		var skewX = getMatrixSkewX(matrix);
		var skewY = getMatrixSkewY(matrix);

		if (false && skewsAreEqual(skewX, skewY)) {
			sprite.rotation = skewX;
		} else {
			sprite.skewX = skewX;
			sprite.skewY = skewY;
		}

		sprite.width = roundToTwip(element.width);
		sprite.height = roundToTwip(element.height);

		var anchorPoint = element.getTransformationPoint();
		sprite.anchorX = anchorPoint.x;
		sprite.anchorY = anchorPoint.y;

		sprite.frame = frame;

		var spriteRect = {'left':sprite.left, 'top':sprite.top, 'right':sprite.right, 'bottom':sprite.bottom};
		var symbol = frame.layer.symbol;
		symbol.updateSizeIfNeedWithRect(spriteRect);

		return sprite;
	}

	FlashExporter.prototype.dumpFlashShape = function(shape, frame) {
		logUp('shape path="' + this.getElementPath(shape, frame) + '"');
		var doc = this.document;

		selectElement(shape, doc);

		if (shape.isGroup) {
			if (isElementsAllShape(shape.members)) {
				log('group all shape');
				
				this.createStaticSymbolWithShapeGroup(shape, frame);

			} else {
				log('group with mc');

				this.dumpGroupFlashShape(shape, frame);
			}
		} else {
			log('drawing shape');
			this.dumpUnGroupFlashShape(shape, frame);
		}
		logDown('shape');
	}

	FlashExporter.prototype.dumpGroupFlashShape = function(group, frame) {
		var doc = this.document;

		doc.enterEditMode('inPlace');
		log('member:' + group.members.length);

		for (var i = group.members.length - 1; i >= 0; i--) {
			var member = group.members[i];
			if (member.elementType == 'shape' && member.isGroup == false) {
				continue;
			}
			this.dumpFlashElement(member, frame);
		}
		
		doc.selectAll();

		var selection = doc.selection;
		selection.forEach(function(elementInGroup){
			if (elementInGroup.elementType != 'shape' || elementInGroup.isGroup) {
				elementInGroup.selected = false;
			}
		});
		
		if (doc.selection.length > 0) {
			this.dumpUnGroupFlashShape(null, frame);
		}

		doc.exitEditMode();
	}

	FlashExporter.prototype.dumpUnGroupFlashShape = function(unGroupShape, frame) {
		log('ungroup shape');
		var doc = this.document;

		
		doc.group(); {
			var shapeInGroup = doc.selection[0];

			this.createStaticSymbolWithShapeGroup(shapeInGroup, frame);

		} doc.unGroup();
	}

	FlashExporter.prototype.createStaticSymbolWithShapeGroup = function(shapeGroup, frame) {
		var doc = this.document;
		//shapeGroup.selected = true;

		if (shapeGroup.width < 2.0 || shapeGroup.height < 2.0) {
			return;
		}

		doc.enterEditMode('inPlace'); {
			var library = doc.library;
			var name = generateImageName(this.imagePrefix);
			var tmpSymbolName = '-----TempSymbolForBitmap----';

			doc.selectAll();
			doc.convertToSymbol("graphic", tmpSymbolName, "center");
			var shapeGraphic = doc.selection[0];
			var transformPoint = {'x':shapeGraphic.transformX, 'y':shapeGraphic.transformY};

			doc.exportInstanceToLibrary(1, name);

			library.addItemToDocument(transformPoint, name);
			var bitmap = doc.selection[0];
			var sprite = this.createSpriteWithElement(bitmap, frame);

			doc.deleteSelection();

			doc.selectAll();
			doc.breakApart();


			// doc.library.deleteItem(tmpSymbolName);
			// doc.library.moveToFolder(this.symbolFolderName, name);

			{//code for new output method
				var library = doc.library;
				library.deleteItem(name);

				var superSymbol = frame.layer.symbol;
				var folderPath = this.symbolFolderName + '/' + superSymbol.name;
				if (!library.itemExists(folderPath)) {
					library.addNewItem('folder', folderPath);
				}

				library.moveToFolder(folderPath, tmpSymbolName);
				library.selectItem(folderPath + '/' + tmpSymbolName);
				library.renameItem(name);

				var shapeItem = library.getSelectedItems()[0];
				shapeItem.addData('width', 'integer', sprite.width);
				shapeItem.addData('height', 'integer', sprite.height);
			}

			frame.addSpriteAtHead(sprite);
		}doc.exitEditMode();

	}

	FlashExporter.prototype.dumpFlashInstance = function(instance, frame) {
		logUp('instance path="' + this.getElementPath(instance, frame) + '"');
		if (instance.instanceType == 'bitmap') {
			this.dumpFlashBitmapInstance(instance, frame);
		} else if (instance.instanceType == 'symbol') {
			var doc = this.document;
			selectElement(instance, doc);

			var symbol = this.symbolLibrary.getSymbolWithItemCreateIfNoExist(instance.libraryItem);
			// if (symbol.isVisited == false) {
			// 	this.dumpSymbol(symbol);
			// }

			{//handle scale info
				var scaleToSuperSymbol = instance.scaleX > instance.scaleY ? instance.scaleX : instance.scaleY;
				scaleToSuperSymbol = roundToTwip2(scaleToSuperSymbol);

				var superSymbol = frame.layer.symbol;
				symbol.updateScaleIfNeedWithSuperSymbol(scaleToSuperSymbol, superSymbol);
			}

			frame.layer.symbol.addSubSymbolIfNeed(symbol);

			var sprite = this.createSpriteWithElement(instance, frame);
			frame.addSpriteAtHead(sprite);
		} else {
			warn('暂不支持这种类型的实例:' + instance.instanceType + '(' + this.getElementPath(instance, frame) + ')');
		}
		logDown('instance');
	}

	FlashExporter.prototype.dumpFlashBitmapInstance = function(bitmapInstance, frame) {
		log('bitmap');
		var doc = this.document;
		selectElement(bitmapInstance, doc);
		var symbol = this.symbolLibrary.getSymbolWithItemCreateIfNoExist(bitmapInstance.libraryItem);
		if (symbol.isVisited == false) {

			var imageName = generateImageName(this.imagePrefix);

			doc.group();doc.enterEditMode('inPlace'); {
				var library = doc.library;

				library.addItemToDocument({'x':0, 'y':0}, symbol.name);

				var bitmap = doc.selection[0];
				symbol.width = bitmap.width;
				symbol.height = bitmap.height;
				symbol.offsetX = 0.0;
				symbol.offsetY = 0.0;
				symbol.frameCount = 1;
				symbol.texture = imageName;
				symbol.isStatic = true;

				doc.deleteSelection();

			} doc.exitEditMode();doc.unGroup();
		}

		var sprite = this.createSpriteWithElement(bitmapInstance, frame);
		frame.addSpriteAtHead(sprite);
	}

	FlashExporter.prototype.getAllLibraryItemThatNeedExportImage = function() {
		var doc = this.document;
		var library = doc.library;

		var symbolsToExport = [];

		this.symbolLibrary.symbols.forEach(function(symbol){
			if (symbol.isStatic) {
				symbolsToExport.push(symbol);
			}
		});

		library.selectItem(this.symbolFolderName);
		var items = library.getSelectedItems();
		items.forEach(function(item){
			if (item.itemType != 'folder') {
				symbolsToExport.push(item);
			}
		});

		return symbolsToExport;
	}

	FlashExporter.prototype.exportImage = function() {
		var doc = this.document;
		var library = doc.library;

		var symbolsToExport = this.getAllLibraryItemThatNeedExportImage();

		var canExportSpriteSheet = getFlashVersionNumber() >= 12;
		if (this.outputSpriteSheetIfPossible && canExportSpriteSheet) {
			this.exportSymbolsToSpriteSheet(symbolsToExport);
		} else {
			this.exportSymbolsToPNGSequence(symbolsToExport);
		}
	}

	var _lastPercentageInt = -1;
	var updatePercentage = function(percentage) {
		var percentageInt = Math.floor(percentage * 100);
		if (Math.abs(percentageInt - _lastPercentageInt) >= 1) {
			_lastPercentageInt = percentageInt;
			fl.trace(percentageInt + '%');
		}
	}

	FlashExporter.prototype.exportSymbolsToSpriteSheet = function(symbols) {
		var spriteSheetExporter = fl.spriteSheetExporter;
		spriteSheetExporter.algorithm = 'basic';//'maxRects'
		spriteSheetExporter.allowTrimming = false;
		spriteSheetExporter.autoSize = true;
		spriteSheetExporter.borderPadding = 2;
		spriteSheetExporter.layoutFormat = 'cocos2dv3';
		spriteSheetExporter.shapePadding = 2;
		spriteSheetExporter.stackDuplicateFrames = true;

		symbols.forEach(function(symbol){
			var imageName = null;
			var symbolItem = null;
			if (symbol instanceof CCSymbol) {
				imageName = symbol.texture;
				symbolItem = symbol.item;
			} else {
				imageName = symbol.name.substr(symbol.name.lastIndexOf('/') + 1, symbol.name.length);
				symbolItem = symbol;
			}

			var imagePath = this.getImageOutputFolderPath();
			if (symbolItem.itemType == 'bitmap') {
				spriteSheetExporter.addBitmap(symbolItem);
			} else {
				spriteSheetExporter.addSymbol(symbolItem);
			}
		}, this);

		var spriteSheetPath = this.getOutputFolderPath() + '/data.jpg';
		log(spriteSheetPath);

		spriteSheetExporter.exportSpriteSheet(spriteSheetPath, 'jpg');

		symbols.forEach(function(symbol){
			var symbolItem = null;
			if (symbol instanceof CCSymbol) {
				symbolItem = symbol.item;
			} else {
				symbolItem = symbol;
			}

			if (symbolItem.itemType == 'bitmap') {
				spriteSheetExporter.removeBitmap(symbolItem);
			} else {
				spriteSheetExporter.removeSymbol(symbolItem);
			}
		});
	}

	FlashExporter.prototype.exportSymbolsToPNGSequence = function(symbols) {
		var doc = this.document;
		var library = doc.library;

		var count = symbols.length;
		symbols.forEach(function(item, index){
			var imageName = null;
			var symbolItem = null;
			if (item instanceof CCSymbol) {
				imageName = item.texture;
				symbolItem = item.item;
			} else {
				imageName = item.name.substr(item.name.lastIndexOf('/') + 1, item.name.length);
				symbolItem = item;
			}

			var imagePath = this.getImageOutputPath(imageName) + '.png';

			if (symbolItem.itemType == 'bitmap') {
				symbolItem.exportToFile(imagePath);
			} else {
				var flashVerNum = getFlashVersionNumber();
				if (flashVerNum >= 12) {
					var matrix = {
						'a': 1,		'b': 0,
						'c': 0,		'd': 1,
						'tx': 0,	'ty': 0,
					};
					symbolItem.exportToPNGSequence(imagePath, 1, 1, matrix);
				} else {
					library.editItem(item.name);

					doc.selectAll();
					var rect = doc.getSelectionRect();
					var width = Math.floor(rect.right - rect.left);
					var height = Math.floor(rect.bottom - rect.top);

					var docOriginalWidth = doc.width;
					var docOriginalHeight = doc.height;

					var applyOffset = {x:-rect.left, y:-rect.top};
					var revertOffset = {x:rect.left, y:rect.top};

					doc.moveSelectionBy(applyOffset);
					doc.width = width;
					doc.height = height;

					doc.exportPNG(imagePath, true, true);

					doc.width = docOriginalWidth;
					doc.height = docOriginalHeight;
					doc.moveSelectionBy(revertOffset);
				}
			}
		}, this);
	}

	FlashExporter.prototype.exportImageWithMaxScale = function() {
		this.symbolLibrary.symbols.forEach(function(symbol){
			// log('[export symbol:' + symbol.name + ']');
			if (symbol.isStatic) {
				this.exportStaticSymbolImage(symbol);
			} else {
				this.exportDynamicSymbolImages(symbol);
			}
		}, this);
	}

	FlashExporter.prototype.exportStaticSymbolImage = function(symbol) {
		var imageName = symbol.texture;
		if (symbol.texture.length == 0) {
			return;
		}
		var imagePath = this.getImageOutputPath(imageName) + '.png';

		var symbolItem = symbol.item;
		if (symbolItem.itemType == 'bitmap') {
			symbolItem.exportToFile(imagePath);
		} else {
			var maxScale = symbol.getMaxScale();

			var width = symbol.width;
			var height = symbol.height;

			var minSize = width < height ? width : height;
			if (minSize * maxScale < 4.0) {
				maxScale = 4.0 / minSize;
			}

			var matrix = {
				'a': 	maxScale,		'b': 	0,
				'c': 	0,				'd': 	maxScale,
				'tx': 	0,				'ty': 	0,
			};

			symbolItem.exportToPNGSequence(imagePath, 1, 1, matrix);
		}
	}

	FlashExporter.prototype.exportDynamicSymbolImages = function(symbol) {
		var doc = this.document;
		var library = doc.library;

		var libraryFolderPath = this.symbolFolderName + '/' + symbol.name;
		if (!library.itemExists(libraryFolderPath)) {
			return;
		}
		library.selectItem(libraryFolderPath);

		var shapeSymbols = library.getSelectedItems();
		shapeSymbols.forEach(function(shapeItem){
			if (shapeItem.itemType == 'folder') {
				return;
			}

			var maxScale = symbol.getMaxScale();

			var width = shapeItem.getData('width');
			var height = shapeItem.getData('height');

			var minSize = width < height ? width : height;
			if (minSize * maxScale < 4.0) {
				maxScale = 4.0 / minSize;
			}
			
			var matrix = {
				'a': 	maxScale,		'b': 	0,
				'c': 	0,				'd': 	maxScale,
				'tx': 	0,				'ty': 	0,
			};

			var imageName = shapeItem.name.substr(shapeItem.name.lastIndexOf('/') + 1, shapeItem.name.length);
			var imagePath = this.getImageOutputPath(imageName) + '.png';
			// log(symbol.name + ', ' + imageName + ':' + maxScale + ', [' + width + ',' + height + '], ' + minSize * maxScale);
			shapeItem.exportToPNGSequence(imagePath, 1, 1, matrix);
		}, this);

		library.selectNone();
	}

	FlashExporter.prototype.exportData = function() {
		return data = this.dataFormatter.formatData(
				this.spriteSheetMetaFileName,
				this.document.frameRate,
				findTheRootElement(this.document).libraryItem.name,
				this.symbolLibrary
			);
	}

	FlashExporter.prototype.getElementPath = function(element, frame) {
		var elementIdentify = "";
		if (element.name && element.name.length > 0) {
			elementIdentify = element.name;
		} else if (element.elementType == 'instance') {
			elementIdentify = element.libraryItem.name;
		} else {
			elementIdentify = ['(x:', element.left, ', y:', element.top, ', w:' + element.width + ', h:' + element.height + ')'].join('');
		}
		return frame.getPath() + '/' + elementIdentify;
	}

	FlashExporter.prototype.traceElement = function(element) {
		var propertys = ["depth", "elementType", "height", "layer", "left", "locked", "matrix", "name", "rotation", "scaleX", "scaleY", "selected", "skewX", "skewY", "top", "transformX", "transformY", "width", "x", "y"];
		log('Element {');
		for (var i = 0; i < propertys.length; i ++) {
			var property = propertys[i];
			if (property == 'matrix') {
				var matrix = element[property];

				//continue;
			}
			log("\t" + property + ':' + element[property]);
		}

		var transfromPoint = element.getTransformationPoint();
		log('\tanchorX:' + transfromPoint.x);
		log('\tanchorY:' + transfromPoint.y);

		if (element.elementType == 'instance') {
			log('\talpha:' + element.colorAlphaPercent);
		}

		var matrix = element['matrix'];
		log('\tmatix:');
		log('\t\t\t' + matrix.a + '\t\t\t' + matrix.b + '\t\t\t' + 0);
		log('\t\t\t' + matrix.c + '\t\t\t' + matrix.d + '\t\t\t' + 0);
		log('\t\t\t' + matrix.tx + '\t\t\t' + matrix.ty + '\t\t\t' + 0);
		log('\tmatix scale:' + getMatrixScaleX(matrix) + ',' + getMatrixScaleY(matrix));
		log('\tscale:' + roundToTwip(getScaleX(matrix)) + ',' + roundToTwip(getScaleY(matrix)));
		log('\tmatix skew:' + getMatrixSkewX(matrix) + ',' + getMatrixSkewY(matrix));
		log('\tskew:' + roundToTwip(getSkewX(matrix)) + ',' + roundToTwip(getSkewY(matrix)));
		log('} Element');
	}

	FlashExporter.prototype.traceFrame = function(frame) {
		var propertys = ["startFrame", "tweenType", "name", "duration"];
		log('Frame {');
		for (var i = 0; i < propertys.length; i ++) {
			var property = propertys[i];
			log("\t" + property + ':' + frame[property]);
		}
		log('} Frame');
	};

})();

//} FlashExporter Classs

var Cocos2dDataFormatter = function () {

};
(function(){

	var formatSpritesData = function(symbolLibrary) {
		var sprites = {};
		symbolLibrary.symbols.forEach(function(symbol, index){
			if (symbol.isStatic) {
				formatStaticSymbol(symbol, sprites);
			} else {
				formatDynamicSymbol(symbol, sprites, symbolLibrary);
			}
		});

		return sprites;
	}

	var formatStaticSymbol = function(symbol, sprites) {
		if (symbol.isStatic && symbol.texture.length == 0) {
			return;
		}

		var sprite = {
			'id': symbol.name,
			'type': 'shape',
			'frameCount': 1,
			'width': symbol.width,
			'height': symbol.height,
			'file': symbol.texture + '.png',
		};

		sprites[sprite.id] = sprite;
	}

	var formatDynamicSymbol = function(symbol, sprites, symbolLibrary) {
		var layersData = [];

		symbol.layers.forEach(function(layer, index){
			var layerData = formatLayer(layer, sprites, symbolLibrary);
			layersData.push(layerData);
		});

		var sprite = {
			'id': symbol.name,
			'type': 'instance',
			'frameCount': symbol.frameCount,
			'width': symbol.width,
			'height': symbol.height,
			'layers': layersData,
		};

		sprites[sprite.id] = sprite;
	}

	var formatLayer = function(layer, sprites, symbolLibrary) {
		var framesData = [];

		layer.keyFrames.forEach(function(keyFrame){
			framesData.push(formatFrame(keyFrame, sprites, symbolLibrary));
		});

		var layerData = {
			'name': layer.name,
			'type': layer.type,
			'frameCount': layer.frameCount,
			'frames': framesData
		};

		if (layer.parentLayerName) {
			layerData.parentLayerName = layer.parentLayerName;
		}

		return layerData;
	}

	var formatFrame = function(frame, sprites, symbolLibrary) {
		var spritesData = [];

		frame.sprites.forEach(function(sprite){
			var spriteData = formatSprite(sprite, sprites, symbolLibrary);
			if (spriteData) {
				spritesData.push(spriteData);
			}
		});


		var frameData = {
			'type': frame.type,
			'startFrame': frame.startFrame,
			'duration': frame.duration,
			'sprites': spritesData
		};

		// if (frame.layer.keyFrames[frame.nextFrame]) {
		// 	frameData.nextFrame = frame.nextFrame;
		// }

		return frameData;
	}

	var roundToTwip = function(value) {
		return Math.round(value * 20) / 20;
	}

	var roundToTwip2 = function(value) {
		return Math.round(value * 200) / 200;
	}

	var isMatrixIdentity = function(matrix) {
		var isFloatEq = function(f1, f2) {
			return Math.abs(f1 - f2) < 0.001;
		}

		var isIdentity =
			isFloatEq(matrix.a, 1.0) &&
			isFloatEq(matrix.b, 0.0) &&
			isFloatEq(matrix.c, 0.0) &&
			isFloatEq(matrix.d, 1.0) &&
			isFloatEq(matrix.tx, 0.0) &&
			isFloatEq(matrix.ty, 0.0);

		return isIdentity;
	}

	var formatSprite = function(sprite, sprites, symbolLibrary) {
		if (sprite.type == 'shape') {
			var librarySpriteData = {
				'id': sprite.id,
				'type': 'shape',
				'frameCount': 1,
				'width': sprite.width,
				'height': sprite.height,
				'file': sprite.texture + '.png',
			};

			sprites[librarySpriteData.id] = librarySpriteData;
		}
		
		var parentSprite = sprite.frame.layer.symbol;

		var frameSpriteData = {
			'id': sprite.id,
			'type': sprite.type,
			'x': roundToTwip(sprite.transformX + parentSprite.offsetX),
			'y': roundToTwip(parentSprite.height - (sprite.transformY + parentSprite.offsetY)),
		};

		if (sprite.type == 'shape') {
			frameSpriteData.anchorX = 0.5;
			frameSpriteData.anchorY = 0.5;
		} else {
			var symbol = symbolLibrary.getSymbolWithName(sprite.id);
			if (symbol.isStatic && symbol.texture.length == 0) {
				return null;
			}

			frameSpriteData.anchorX = roundToTwip((sprite.anchorX + symbol.offsetX) / symbol.width);
			frameSpriteData.anchorY = roundToTwip((symbol.height - (sprite.anchorY + symbol.offsetY)) / symbol.height);
		}

		if (!isMatrixIdentity(sprite.matrix)) {
			var matrix = sprite.matrix;
			var matrixData = {
				'a': roundToTwip2(matrix.a),
				'b': roundToTwip2(matrix.b),
				'c': roundToTwip2(matrix.c),
				'd': roundToTwip2(matrix.d),
				'tx': roundToTwip2(matrix.tx),
				'ty': roundToTwip2(matrix.ty),
			};
			frameSpriteData.matrix = matrixData;
		}

		if (sprite.type == 'graphic') {
			frameSpriteData.firstFrame = sprite.firstFrame;
			frameSpriteData.loop = sprite.loop;
		}

		if (sprite.scaleX != 1.0) {
			frameSpriteData.scaleX = sprite.scaleX;
		}
		if (sprite.scaleY != 1.0) {
			frameSpriteData.scaleY = sprite.scaleY;
		}
		if (sprite.skewX != 0.0) {
			frameSpriteData.skewX = sprite.skewX;
		}
		if (sprite.skewY != 0.0) {
			frameSpriteData.skewY = sprite.skewY;
		}
		if (sprite.rotation != 0.0) {
			frameSpriteData.rotation = sprite.rotation;
		}

		if (sprite.alpha != 1.0) {
			frameSpriteData.alpha = sprite.alpha;
		}

		if (false && sprite.action) {
			frameSpriteData.action = {};
			var actionKeyMap = {
				'transformXBy' : 'moveXBy',
				'transformYBy' : 'moveYBy',
				'skewXBy' : 'skewXBy',
				'skewYBy' : 'skewYBy',
				'scaleXBy' : 'scaleXBy',
				'scaleYBy' : 'scaleYBy',
				'rotationBy' : 'rotateBy',
				'alphaBy' : 'alphaBy'
			};
			for (var key in actionKeyMap) {
				if (sprite.action[key] != 0) {
					frameSpriteData.action[actionKeyMap[key]] = sprite.action[key];
				}
			}
		}

		return frameSpriteData;
	}

	Cocos2dDataFormatter.prototype.formatData = function(spriteSheetFileName, frameRate, rootId, symbolLibrary) {
		var sprites = formatSpritesData(symbolLibrary);
		var jsonData = {
			'textures': [spriteSheetFileName],
			'frameRate':frameRate,
			'rootId': rootId,
			'sprites': sprites,
		};

		return JSON.stringify(jsonData);
	}

})();
