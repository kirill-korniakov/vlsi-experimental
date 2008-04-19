<?xml version="1.0" encoding="ISO-8859-1"?>
<xsl:stylesheet version="2.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
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
@echo off
echo [Main] Changing config to PDF
perl -i.bak -nle "s/doctype=\"\S+\"/doctype=\"PDF\"/i; print" ..\..\config.xml

time /t

echo ^&lt;all^&gt; &gt; tmp.xml
echo [Main] Creating contact page
type Presentation.xml &gt;&gt; tmp.xml

echo [Main] Creating TOC
call saxon8 ..\..\CornerList.xml ..\xsl\TOC_xml.xsl &gt;&gt; tmp.xml

<xsl:for-each select="Corner">
	<xsl:variable name="Corner" select="@Name"/>
			
	<xsl:variable name="XmlLib" select="document(@File)/Library"/>
	<xsl:for-each select="$XmlLib/Profilings/Plot[@File]|$XmlLib/Profilings/Plot/Plot[@File]">
		echo <xsl:value-of select="concat('[', $Corner, '/Profilings] ', @File)"/><xsl:text>
		</xsl:text>
		<xsl:value-of select="concat('call saxon8 ../../',@File, '_', $Corner, '.xml ..\xsl\ProfilePlot_xml.xsl &gt;&gt; tmp.xml')"/><xsl:text>
		</xsl:text>
	</xsl:for-each>

	<xsl:for-each select="$XmlLib/TimingSummary/Entry">
		echo <xsl:value-of select="concat('[', $Corner, '/TimingSummary] ', @Link)"/><xsl:text>
		</xsl:text>
		<xsl:value-of select="concat('call saxon8 ../../', @Link, '_', $Corner, 
				'.xml ../../resources/xsl/GenericTable_xml.xsl', 
				' &gt;&gt; tmp.xml')"/><xsl:text>
		</xsl:text>
	</xsl:for-each>

	<xsl:for-each select="$XmlLib/Cells/CellType/Cell">
		echo <xsl:value-of select="concat('[', $Corner, '/Cells/', 
			../@Name, '] ', @Name)"/><xsl:text>
		</xsl:text>
		<xsl:value-of select="concat('call saxon8 ../../Cells/',
			@Name, '_', $Corner, 
			'.xml ..\xsl\Cell_xml.xsl &gt;&gt; tmp.xml')"/><xsl:text>
		</xsl:text>
	</xsl:for-each>
</xsl:for-each>
		
echo [Main] Adding extra info
call saxon8 ..\..\CornerList.xml  ExtraInfo.xsl &gt;&gt; tmp.xml

echo ^&lt;/all^&gt; &gt;&gt; tmp.xml

echo [Main] Remove Extra Tags
perl -i.bak -nle "s/\&lt;\?[^\&lt;]*\?\&gt;/\n\n\n/gi; print" tmp.xml

echo [Main] Copy to all.xml
copy tmp.xml all.xml

time /t

echo [Main] Generating PDF
rem call fop -xsl HTML2FO.xsl -pdf all.pdf -xml all.xml 2&gt; err
call fop_old -xsl HTML2FO.xsl -pdf all.pdf -xml all.xml &gt; err

echo [Main] Changing config to html
perl -i.bak -nle "s/doctype=\"\S+\"/doctype=\"Html\"/i; print" ..\..\config.xml

time /t

	</xsl:template>
</xsl:stylesheet>
