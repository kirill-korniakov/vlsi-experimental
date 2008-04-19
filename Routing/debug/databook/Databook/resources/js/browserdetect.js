function verifyBrowser() {
	var ret=false;
	if(navigator.userAgent.indexOf("Firefox")!=-1){
		var versionindex=navigator.userAgent.indexOf("Firefox")+8
		if (parseInt(navigator.userAgent.charAt(versionindex))>=1)
			ret = true;
	}

	version=0;
	if (navigator.appVersion.indexOf("MSIE")!=-1){
		temp=navigator.appVersion.split("MSIE");
		version=parseFloat(temp[1]);
		if (version>=5.5) //NON IE browser will return 0
			ret = true;
	}
		
	if (!ret) {
		body = document.getElementsByTagName("BODY")[0];
		body.innerHTML = '<p class="comment">Better visualized with '+
			getSupportedBrowsers() + '.</p>' + body.innerHTML;
	}
	return ret;
}

function getSupportedBrowsers() {
	return 'Firefox 1.5+ and IE 5.5+';
}