<?xml version="1.0" encoding="ISO-8859-1"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
	<!-- 
	    Copyright (C) 2006, Nangate Inc.
	    All rights reserved.                           
	
		Copyright Notice:

			This document contains confidential and proprietary information.
			Reproduction or usage of this document, in part or whole, by any means,
			electrical, mechanical, optical, chemical or otherwise is prohibited,
			without written permission from Nangate Inc.

			The information contained herein is protected by Danish and international  
			copyright laws.                                                            
		
		Author: 
			Joao Daniel Togni
	-->
	<xsl:output method="text"/>
	
    <!-- Starting from the root -->
    <xsl:template match="CornerList">
bbdir=`cd ../..;pwd`
XSLTPROC="xsltproc"
XSL=""
XML=""
		
echo "[`date +%T` Main] Changing config to PDF"
perl -i.bak -nle "s/doctype=\"\S+\"/doctype=\"PDF\"/i; print" ${bbdir}/config.xml

echo "&lt;all&gt;" &gt; tmp.xml
echo "[`date +%T` Main] Creating contact page"
cat Presentation.xml &gt;&gt; tmp.xml

echo "[`date +%T` Main] Creating TOC"
${XSLTPROC} ${XSL} ${bbdir}/resources/xsl/TOC_xml.xsl ${XML} ${bbdir}/CornerList.xml &gt;&gt; tmp.xml 2&gt; err.scr

<xsl:for-each select="Corner">
	<xsl:variable name="Corner" select="@Name"/>
	echo "<xsl:value-of select="concat('[`date +%T` ', $Corner, '] Create Corner')"/>"<xsl:text>
</xsl:text>
	
	<xsl:variable name="XmlLib" select="document(@File)/Library"/>
	<xsl:for-each select="$XmlLib/Profilings/Plot[@File]
			| $XmlLib/Profilings/Plot/Plot[@File]">
		echo "<xsl:value-of select="concat('[`date +%T` ', $Corner, '/Profilings] ', @File)"/>"<xsl:text>
</xsl:text>
		<xsl:value-of select="concat('${XSLTPROC} ${XSL} ${bbdir}/resources/xsl/ProfilePlot_xml.xsl',
				' ${XML} ${bbdir}/', @File, '_', $Corner, '.xml ', 
				' &gt;&gt; tmp.xml 2&gt; err.scr')"/><xsl:text>
</xsl:text>
	</xsl:for-each>

	<xsl:for-each select="$XmlLib/TimingSummary/Entry">
		echo "<xsl:value-of select="concat('[`date +%T` ', $Corner, '/TimingSummary] ', @Link)"/>"<xsl:text>
</xsl:text>
		<xsl:value-of select="concat('${XSLTPROC} ${XSL} ${bbdir}/resources/xsl/GenericTable_xml.xsl',
				' ${XML} ${bbdir}/', @Link, '_', $Corner, '.xml', 
				' &gt;&gt; tmp.xml 2&gt; err.scr')"/><xsl:text>
</xsl:text>
	</xsl:for-each>

	<xsl:for-each select="$XmlLib/Cells/CellType/Cell">
		echo "<xsl:value-of select="concat('[`date +%T` ', $Corner, '/Cells/', 
			../@Name, '] ', @Name)"/>"<xsl:text>
</xsl:text>
		<xsl:value-of select="concat('${XSLTPROC} ${XSL} ${bbdir}/resources/xsl/Cell_xml.xsl',
			' ${XML} ${bbdir}/Cells/', @Name, '_', $Corner, '.xml', 
			' &gt;&gt; tmp.xml 2&gt; err.scr')"/><xsl:text>
</xsl:text>
	</xsl:for-each>
</xsl:for-each>
		
echo "[`date +%T` Main] Adding extra info"
${XSLTPROC} ${XSL} ExtraInfo.xsl ${XML} ${bbdir}/CornerList.xml &gt;&gt; tmp.xml 2&gt; err.scr

echo "&lt;/all&gt;" &gt;&gt; tmp.xml

echo "[`date +%T` Main] Remove Extra Tags"
perl -i.bak  -nle "s/\&lt;\?[^\&lt;]*\?\&gt;/\n\n\n/gi; print" tmp.xml

echo "[`date +%T` Main] Copy to all.xml"
cp tmp.xml all.xml

echo "[`date +%T` Main] Generating PDF"
fop.sh -xsl HTML2FO.xsl -pdf all.pdf -xml all.xml &gt; err

echo "[`date +%T` Main] Changing config to html"
perl -i.bak -nle "s/doctype=\"\S+\"/doctype=\"Html\"/i; print" ${bbdir}/config.xml

	</xsl:template>
</xsl:stylesheet>
