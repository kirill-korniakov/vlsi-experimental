//
//     Copyright (C) 2006, Nangate Inc.
//     All rights reserved.                           
//
//	Copyright Notice:
//	
//		This document contains confidential and proprietary information.		
//		Reproduction or usage of this document, in part or whole, by any means,
//		electrical, mechanical, optical, chemical or otherwise is prohibited,
//		without written permission from Nangate Inc.
//		
//		The information contained herein is protected by Danish and international  
//		copyright laws.                                                            /
//	
//	Author: 
//		Joao Daniel Togni
//
//  Version: 
//		1.0
// 

//
// Main Javascript
// - "import" other javascripts
//

loadScript('resources/js/browserdetect.js'      , 'javascript');
loadScript('resources/js/highlight.js'          , 'javascript');
loadScript('resources/js/collapse.js'           , 'javascript');
loadScript('resources/js/query.js'              , 'javascript');
loadScript('resources/js/tooltip.js'            , 'javascript');
loadScript('resources/js/cookies.js'            , 'javascript');
loadScript('resources/js/config.js'             , 'javascript');
loadScript('resources/js/ctp.js'                , 'javascript');

function NanInit() {	
	queryString();
	verifyBrowser();
}

function showPlot(path, str, title, ctx, width, height) {
	var myWin = window.open(
			path+'/resources/js/plot.html','',
			'width='+width+',height='+height+',top=100,left=100');
	myWin.mplot=str;
	myWin.mtitle=title;
	myWin.mctx=ctx;
}

function readPlot() {
	var str = window.mplot;
	var title = window.mtitle;
	var context = window.mctx;
	
	window.document.body.innerHTML =
		window.document.body.innerHTML+
		context+'<br/><center><img src="'+str+'"/></center>';
		
	window.document.getElementById("_title").innerHTML = title;
}

function showPlotList(path, str, title, ctx, width, height, count) {
  var myWin = window.open(path + '/resources/js/plot_list.html', '' , 'width=' + width + ',height=' + height + ',top=100,left=100');
  myWin.mplot=str;
  myWin.mtitle=title;
  myWin.mctx=ctx;
  myWin.current = 1;
  myWin.plotCount = count;
}

function readPlotList() {
  var str = window.mplot;
  var title = window.mtitle;
  var context = window.mctx;
  var current = window.current;
  var plotCount = window.plotCount;
  
  baseHtml = '<table class="navigation" width="100%">' +
             '  <tr>' +
             '    <td align="left" id="_title">' + title + '</td>' +
             '    <td align="right">' +
             '      <a href="http://www.nangate.com" target="_blank"><img src="../image/Nangatelogo_small.gif" border="0"/></a>' +
             '      <br/><br/>' +
             '      <span class="comment">Copyright ® 2006 Nangate Inc.</span>' +
             '    </td>' +
             '  </tr>';
  
  if (plotCount > 1) {
    baseHtml += '<tr><td>';
    if (current > 1)
      baseHtml += '<img src="../image/first.gif" onClick="window.current = 1; readPlotList();"/>' +
                  '<img src="../image/blank.gif"/>' + 
                  '<img src="../image/previous.gif" onClick="window.current--; readPlotList();"/>';
    else
      baseHtml += '<img src="../image/blank.gif"/><img src="../image/blank.gif"/><img src="../image/blank.gif"/>';
    baseHtml += '<img src="../image/blank.gif"/>' + current + ' of ' + plotCount + '<img src="../image/blank.gif"/>';
    if (current != plotCount)
      baseHtml += '<img src="../image/next.gif" onClick="window.current++; readPlotList();"/>' +
                  '<img src="../image/blank.gif"/>' +
                  '<img src="../image/last.gif" onClick="window.current = plotCount; readPlotList();"/>';
    baseHtml += '</td></tr>';
  }
       
  baseHtml += '</table><hr class="navigation_tb">';
  window.document.body.innerHTML = baseHtml + context + '<br/><center><img name="plot" src="' + str + window.current + '.png"/></center>';
}

function loadScript(name,type) {
	if (type == 'javascript') {
		elem = 'script';
	} else if (type == 'css') {
		elem = 'link';
	}
	
	var script = document.createElement(elem);
	script.type = 'text/'+type ;
	
	if (type == 'javascript') {
		script.src = name;
	} else if (type == 'css') {
		script.href = name;
		script.rel = 'stylesheet';
	}	
	document.getElementsByTagName('head')[0].appendChild(script); 
	
	name = '../'+name;
	script = document.createElement(elem);
	script.type = 'text/'+type ;
	if (type == 'javascript') {
		script.src = name;
	} else if (type == 'css') {
		script.href = name;
		script.rel = 'stylesheet';
	}	
	document.getElementsByTagName('head')[0].appendChild(script); 
}

function openFrame(str) {
	var params = str.split("%");
	myWin = window.open('','','');
	myWin.document.write('<html><head></head><frameset cols="50%,50%">\n');
	myWin.document.write('<frame src="'+params[2]+params[4]+params[6]+'" scrolling="auto"/>\n');
	myWin.document.write('<frame src="'+params[3]+params[5]+params[7]+'" scrolling="auto"/>\n');
	myWin.document.write('</frameset></html>\n');
}

/*
Function to change the contents of a look-up table based on a combo box selection.
*/
function updateTable(comboBox, tableId, fallCount, riseCount) {
  table = document.getElementById(tableId);
  unit = comboBox.options[comboBox.selectedIndex].getAttribute('unit');
  if (fallCount > 0) {
    fixUnit(table.rows[2].cells[1].childNodes[0], unit);
    if (riseCount > 0)
      fixUnit(table.rows[2 + fallCount].cells[0].childNodes[0], unit);
  } else if (riseCount > 0)
    fixUnit(table.rows[2].cells[1].childNodes[0], unit);
  for (i in table.rows) {
    cells = table.rows[i].cells;
    for (j in cells) {
      try {
        if (cells[j].tagName == 'TD') {
          cell = cells[j].getElementsByTagName('div')[0];
          tolExc = cell.getAttribute('t' + comboBox.options[comboBox.selectedIndex].value);
          
          // Fix highlight first
          textNode = cell.childNodes[0];
          if (textNode.nodeName == '#text') {
            if (tolExc == 'up' || tolExc == 'down') {
              spanNode = document.createElement('span');
              attr = document.createAttribute('class');
              if (tolExc == 'up')
                attr.value = 'errorUp';
              else
                attr.value = 'errorDown';
              spanNode.setAttributeNode(attr);
              spanNode.appendChild(textNode.cloneNode(true));
              cell.replaceChild(spanNode, textNode);
              textNode = spanNode.childNodes[0];
            }
          } else {
            spanNode = textNode;
            textNode = spanNode.childNodes[0];
            
            if (tolExc == 'up' && spanNode.getAttribute('class') == 'errorDown')
              spanNode.setAttribute('class', 'errorUp');  
            else if (tolExc == 'down' && spanNode.getAttribute('class') == 'errorUp')
              spanNode.setAttribute('class', 'errorDown');
            else if (tolExc != 'up' && tolExc != 'down')
              cell.replaceChild(textNode, spanNode);
          }
          
          textNode.nodeValue = cell.getAttribute(comboBox.options[comboBox.selectedIndex].value);
        }
      } catch(err) {}
    }
  }
}

/*
Helper function to set the unit information in a table header. Used only by 'updateTable'.
*/
function fixUnit(node, unit) {
  if (node.nodeValue.indexOf('[') != -1)
    node.nodeValue = node.nodeValue.slice(0, node.nodeValue.indexOf('['));
  if (unit != '')
    node.nodeValue += '[' + unit + ']';
}
