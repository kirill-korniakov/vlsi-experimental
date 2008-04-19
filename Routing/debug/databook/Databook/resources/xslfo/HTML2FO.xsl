<?xml version="1.0" encoding="ISO-8859-1"?>
<xsl:stylesheet version="1.0" 
    xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
    xmlns:fo="http://www.w3.org/1999/XSL/Format" 
    xmlns:fox="http://xml.apache.org/fop/extensions">
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
	
    <xsl:output method="xml"/>
    <xsl:include href="TOC2FO.xsl"/>
    <xsl:include href="Table2FO.xsl"/>
    <xsl:variable name="TOCPos" select="2"/>
	<xsl:variable name="Config" select="document('../../config.xml')/config"/>
    
	<!-- Create root elements -->    
    <xsl:template match="/">
        <fo:root xmlns:fo="http://www.w3.org/1999/XSL/Format">
            <fo:layout-master-set>
                <fo:simple-page-master master-name="simpleA4" 
                    page-height="29.7cm" page-width="21cm" margin-top="1cm"
                    margin-bottom="1cm" margin-left="1cm" margin-right="1cm" >
                    <fo:region-body margin-top="1.5cm" margin-bottom="1.5cm"/>
                    <fo:region-before extent="1.5cm"/>
                    <fo:region-after extent="1.5cm"/>
                </fo:simple-page-master>
                <fo:simple-page-master master-name="landA4" 
                    page-height="21cm" page-width="29.7cm" margin-top="1cm"
                    margin-bottom="1cm" margin-left="1cm" margin-right="1cm" >
                    <fo:region-body margin-top="1.5cm" margin-bottom="1.5cm"/>
                    <fo:region-before extent="1.5cm"/>
                    <fo:region-after extent="1.5cm"/>
                </fo:simple-page-master>
            </fo:layout-master-set>
            <xsl:call-template name="CreateOutline"/>
            <xsl:call-template name="CreateCoverPage"/>
            <xsl:apply-templates select="*"/>
        </fo:root>
    </xsl:template>

    <xsl:template name="CreateCoverPage">
        <fo:page-sequence>
			<xsl:attribute name="master-reference"><xsl:value-of select="$Config/pdf/page_type"/></xsl:attribute>
            <xsl:call-template name="CreateHeaderFooter">
                 <xsl:with-param name="Title" select="''"/>
            </xsl:call-template>
            <fo:flow flow-name="xsl-region-body">
                <fo:block font-size="40pt" color="#4A7091" font-weight="bold"
                        margin-top="2cm" break-before="page"
						space-after="5pt" space-before="5cm"
                        id="cover_page" text-align="center">
                    <xsl:value-of select="/all/html[position()=$TOCPos]/head/title"/>
                </fo:block>
            </fo:flow>
        </fo:page-sequence>
    </xsl:template>
    
    <xsl:template name="CreateHeaderFooter">
        <xsl:param name="Title"/>
        <fo:static-content flow-name="xsl-region-before">
            <fo:block space-before="0cm" space-after="0cm">
	            <fo:table table-layout="fixed" width="100%">
		                <fo:table-column/>
			        <fo:table-column/>
				    <fo:table-column/>
				    <fo:table-body>
				        <fo:table-row>
				            <fo:table-cell>
				                <fo:block keep-with-next="always" text-align="left">
					                <fo:external-graphic src="../image/Nangatelogo_small.gif"/>
						        </fo:block>
							</fo:table-cell>
							<fo:table-cell><fo:block keep-with-next="always"></fo:block></fo:table-cell>
	                        <fo:table-cell>
	                            <fo:block keep-with-next="always" text-align="right">
<!--                                <fo:external-graphic src="../image/Nangate_CoolIC_smaller.gif"/>
-->                            </fo:block>
	                        </fo:table-cell>
		                </fo:table-row>
			        </fo:table-body>
	            </fo:table>
	            <fo:block space-before="-8pt" text-align="center">
	                <fo:leader leader-length="100%" leader-pattern="rule"/>
	            </fo:block>
            </fo:block>
        </fo:static-content>
        <fo:static-content flow-name="xsl-region-after">
            <fo:block text-align="center">
                <fo:leader leader-length="100%" leader-pattern="rule"/>
            </fo:block>
            <fo:table table-layout="fixed" width="100%" >
                <fo:table-column/>
                <fo:table-column/>
                <fo:table-column/>
                <fo:table-body>
                    <fo:table-row>
                        <fo:table-cell>
                            <fo:block text-align="left">
                                <xsl:value-of select="/all/html[position()=$TOCPos]/head/title"/>
	                            <fo:block font-size="8pt">
	                                Build Date <xsl:value-of select="/all/extra/BuildDate"/>
								</fo:block>
                            </fo:block>
                        </fo:table-cell>
                        <fo:table-cell>
                            <fo:block text-align="center"><xsl:value-of select="$Title"/></fo:block>
                        </fo:table-cell>
                        <fo:table-cell>
                            <fo:block line-height="14pt" font-size="10pt" text-align="right">Page
                                <fo:page-number/> of <fo:page-number-citation ref-id="last-page"/></fo:block>
                        </fo:table-cell>
                    </fo:table-row>
                </fo:table-body>
            </fo:table>
        </fo:static-content>
    </xsl:template>
    
	<!-- Create every new section (1 for each html) -->    
    <xsl:template match="body">
        <xsl:variable name="title" select="normalize-space(../head/title/text())"/>
        <xsl:variable name="toc" select="/all/html[position()=$TOCPos]/body/ul/li/span"/>
        <xsl:variable name="var1" select="$toc/ul/li/span/ul/li/a[@href=$title]"/>
        <xsl:variable name="var2" select="$toc/ul/li/span/ul/li/ul/li/a[@href=$title]"/>
        <xsl:variable name="Title">
            <xsl:choose>
                <xsl:when test="..=/all/html[position()=$TOCPos]">Table of Contents</xsl:when>
                <xsl:when test="..=/all/html[position()=1]">
					<xsl:value-of select="$title"/>
				</xsl:when>
                <xsl:otherwise>
                    <xsl:for-each select="$var1|$var2"> 
                        <xsl:number level="multiple" count="li" format="1.1"/>
                    </xsl:for-each><xsl:text>. </xsl:text>
                    <xsl:value-of select="$var1/text()|$var1/p/text|$var2/text()|$var2/p/text()"/>
                </xsl:otherwise>
            </xsl:choose>
        </xsl:variable>
        <fo:page-sequence>
            <xsl:attribute name="master-reference"><xsl:value-of select="$Config/pdf/page_type"/></xsl:attribute>
            <xsl:call-template name="CreateHeaderFooter">
                <xsl:with-param name="Title" select="$Title"/>
            </xsl:call-template>
            <fo:flow flow-name="xsl-region-body">
                <fo:block font-size="10pt" color="#000000" 
						space-before="5pt" space-after="5pt" break-before="page">
                    <xsl:call-template name="CreateSectionTitle">
						<xsl:with-param name="Title" select="$Title"/>
					</xsl:call-template>
                    <xsl:apply-templates/>
                </fo:block>
				<xsl:if test="..=../../html[position()=last()]">
					<fo:block id="last-page"/>
				</xsl:if>
            </fo:flow>
        </fo:page-sequence>
    </xsl:template>

    <xsl:template name="CreateSectionTitle">
		<xsl:param name="Title"/>
        <xsl:variable name="title"  select="../head/title/text()"/>
		
        <xsl:variable name="toc"    select="/all/html[position()=$TOCPos]/body/ul/li/span"/>
        <xsl:variable name="var1"   select="$toc/ul/li/span/ul/li/ul/li/a[@href=$title]"/>
        <xsl:variable name="pvar1"  select="count($var1/../preceding-sibling::li)"/>
        <xsl:variable name="pvar1b" select="count($var1/../../../preceding-sibling::li)"/>
        <xsl:variable name="pvar1c" select="count($var1/../../../../../../preceding-sibling::li)"/>

        <xsl:variable name="var2"   select="$toc/ul/li/span/ul/li/a[@href=$title]"/>
        <xsl:variable name="pvar2"  select="count($var2/../preceding-sibling::li)"/>
        <xsl:variable name="pvar2b" select="count($var2/../../../../preceding-sibling::li)"/>
		
        <xsl:variable name="pos"    select="count(../preceding-sibling::*)"/>
		
        <xsl:attribute name="id">
            <xsl:value-of select="$title"/>
        </xsl:attribute>
		
		<xsl:if test="$var2 and $pvar2=0 and $pos>=$TOCPos">
			<xsl:if test="$pvar2b=0">
				<xsl:call-template name="CreateSecTitle">
					<xsl:with-param name="text"  select="$var2/../../../../../../../a"/>
					<xsl:with-param name="li"    select="$var2/../../../../../.."/>
					<xsl:with-param name="level" select="1"/>
				</xsl:call-template>
			</xsl:if>
			<xsl:call-template name="CreateSecTitle">
				<xsl:with-param name="text"  select="$var2/../../../../a"/>
				<xsl:with-param name="li"    select="$var2/../../.."/>
				<xsl:with-param name="level" select="2"/>
			</xsl:call-template>
		</xsl:if>
		<xsl:if test="$var1 and $pvar1=0 and $pos>=$TOCPos">
			<xsl:if test="$pvar1b=0">
				<xsl:if test="$pvar1c=0">
					<xsl:call-template name="CreateSecTitle">
						<xsl:with-param name="text"  select="$var1/../../../../../../../../../a"/>
						<xsl:with-param name="li"    select="$var1/../../../../../../.."/>
						<xsl:with-param name="level" select="1"/>
					</xsl:call-template>
				</xsl:if>
				<xsl:call-template name="CreateSecTitle">
					<xsl:with-param name="text"  select="$var1/../../../../../../a"/>
					<xsl:with-param name="li"    select="$var1/../../../../.."/>
					<xsl:with-param name="level" select="2"/>
				</xsl:call-template>
			</xsl:if>
			<xsl:call-template name="CreateSecTitle">
				<xsl:with-param name="text"    select="$var1/../../../a"/>
				<xsl:with-param name="li"    select="$var1/../../.."/>
				<xsl:with-param name="level" select="3"/>
			</xsl:call-template>
		</xsl:if>
		
        <fo:block>
			<xsl:variable name="level">
				<xsl:choose>
					<xsl:when test="$var2">3</xsl:when>
					<xsl:when test="$var1">4</xsl:when>
					<xsl:otherwise>1</xsl:otherwise>
				</xsl:choose>
			</xsl:variable>
			<xsl:call-template name="CreateFontProperties">
				<xsl:with-param name="class" select="concat('TOC', $level)"/>
			</xsl:call-template>
	        <xsl:value-of select="$Title"/>
		</fo:block>
    </xsl:template>
	
	<xsl:template name="CreateSecTitle">
		<xsl:param name="level"/>
		<xsl:param name="text"/>
		<xsl:param name="li"/>
		<xsl:param name="t" select="$text/text()|$text/p/text()"/>
        <fo:block>
			<xsl:call-template name="CreateFontProperties">
				<xsl:with-param name="class" select="concat('TOC', $level)"/>
			</xsl:call-template>
            <xsl:attribute name="id">
                <xsl:value-of select="$text/@href"/>
            </xsl:attribute>
            <xsl:for-each select="$li"> 
                <xsl:number level="multiple" count="li" format="1.1"/>
            </xsl:for-each>
            <xsl:text>. </xsl:text>
			<xsl:choose>
				<xsl:when test="$t='Corner NoCorner'">
					<xsl:value-of select="/all/html[position()=$TOCPos]/head/title"/>
				</xsl:when>
				<xsl:otherwise>
					<xsl:value-of select="$t"/>
				</xsl:otherwise>
			</xsl:choose>
        </fo:block>
	</xsl:template>

    
    <xsl:template match="hr"/>
    
    <xsl:template match="a">
		<xsl:choose>
        <xsl:when test="@href and substring(@href,1,1)!='#'">
			<fo:inline>
				<xsl:call-template name="CreateFontProperties">
					<xsl:with-param name="class" select="'Link'"/>
				</xsl:call-template>
	            <fo:basic-link>
					<xsl:choose>
						<xsl:when test="substring(@href,1,1)='@'">
				            <xsl:attribute name="internal-destination">
					            <xsl:value-of select="substring(@href,2)"/>
							</xsl:attribute>
						</xsl:when>
						<xsl:otherwise>
				            <xsl:attribute name="external-destination">
					            <xsl:value-of select="@href"/>
							</xsl:attribute>
						</xsl:otherwise>
					</xsl:choose>
		            <xsl:apply-templates/>
	            </fo:basic-link>
			</fo:inline>
        </xsl:when>
		<xsl:otherwise><xsl:apply-templates/></xsl:otherwise>
		</xsl:choose>
    </xsl:template>
	
    <xsl:template match="img">
        <fo:external-graphic>
            <xsl:attribute name="src">
				<xsl:choose>
					<xsl:when test="substring(@src, 1, 1)='@'">
						<xsl:value-of select="substring(@src, 2)"/>
					</xsl:when>
					<xsl:otherwise>
						<xsl:value-of select="@src"/>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:attribute>
		</fo:external-graphic>
    </xsl:template>

	<xsl:template match="text()"><xsl:value-of select="."/></xsl:template>
	
    <xsl:template match="span|div|p|sup">
        <xsl:if test="not(text()) or text()!='Summary'">
			<xsl:call-template name="PrintText"/>
        </xsl:if>
    </xsl:template>

	<!-- process all simple elements according to class info -->	
	<xsl:template name="PrintText">
        <xsl:choose>
            <xsl:when test="name(.)='sup'">
                <fo:inline baseline-shift="super" font-size=".75em">
					<xsl:apply-templates/></fo:inline>
            </xsl:when>
            <xsl:when test="@class='error' or @class='bold' or name(.)='span'">
                <fo:inline>
					<xsl:call-template name="CreateFontProperties">
						<xsl:with-param name="class" select="@class"/>
					</xsl:call-template>
					<xsl:apply-templates/></fo:inline>
            </xsl:when>
            <xsl:when test="@class='branch'"><xsl:apply-templates/></xsl:when>
            <xsl:when test="@class='Title'"></xsl:when>
            <xsl:when test="@class='Title_no_sp'"></xsl:when>
            <xsl:when test="@class='tooltip'"></xsl:when>
            <xsl:otherwise>
                <fo:block>
					<xsl:call-template name="CreateFontProperties">
						<xsl:with-param name="class" select="@class"/>
					</xsl:call-template>
					<xsl:apply-templates/>
				</fo:block>
            </xsl:otherwise>
        </xsl:choose>
	</xsl:template>
	
	<xsl:template name="CreateFontProperties">
		<xsl:param name="class"/>
		<xsl:if test="$class='TOC1' or $class='TOC2' or $class='TOC3' or $class='TOC4'">
			<xsl:attribute name="font-weight">bold</xsl:attribute>
			<xsl:attribute name="space-after">5pt</xsl:attribute>
			<xsl:attribute name="space-before">5pt</xsl:attribute>
		</xsl:if>
        <xsl:choose>
            <xsl:when test="$class='TOC1'">
				<xsl:attribute name="font-size">20pt</xsl:attribute>
				<xsl:attribute name="color">
					<xsl:value-of select="$Config/pdf/toc1_color"/>
				</xsl:attribute>
            </xsl:when>
            <xsl:when test="$class='TOC2'">
				<xsl:attribute name="font-size">18pt</xsl:attribute>
				<xsl:attribute name="color">
					<xsl:value-of select="$Config/pdf/toc2_color"/>
				</xsl:attribute>
            </xsl:when>
            <xsl:when test="$class='TOC3'">
				<xsl:attribute name="font-size">16pt</xsl:attribute>
				<xsl:attribute name="color">
					<xsl:value-of select="$Config/pdf/toc3_color"/>
				</xsl:attribute>
            </xsl:when>
            <xsl:when test="$class='TOC4'">
				<xsl:attribute name="font-size">14pt</xsl:attribute>
				<xsl:attribute name="color">
					<xsl:value-of select="$Config/pdf/toc4_color"/>
				</xsl:attribute>
            </xsl:when>
            <xsl:when test="$class='Caption'">
				<xsl:attribute name="font-weight">bold</xsl:attribute>
				<xsl:attribute name="space-after">10pt</xsl:attribute>
				<xsl:attribute name="space-before">20pt</xsl:attribute>
				<xsl:attribute name="font-size">16pt</xsl:attribute>
				<xsl:attribute name="color">
					<xsl:value-of select="$Config/pdf/title_color"/>
				</xsl:attribute>
            </xsl:when>
            <xsl:when test="$class='SubCaption'">
				<xsl:attribute name="font-weight">bold</xsl:attribute>
				<xsl:attribute name="space-after">5pt</xsl:attribute>
				<xsl:attribute name="space-before">10pt</xsl:attribute>
				<xsl:attribute name="font-size">14pt</xsl:attribute>
				<xsl:attribute name="color">
					<xsl:value-of select="$Config/pdf/title_color"/>
				</xsl:attribute>
            </xsl:when>
            <xsl:when test="$class='bold'">
				<xsl:attribute name="font-weight">bold</xsl:attribute>
            </xsl:when>
            <xsl:when test="$class='error'">
				<xsl:attribute name="font-weight">bold</xsl:attribute>
				<xsl:attribute name="color">#FF0000</xsl:attribute>
            </xsl:when>
            <xsl:when test="$class='num'"></xsl:when>
            <xsl:when test="$class='tooltip'"></xsl:when>
            <xsl:when test="$class='branch'"></xsl:when>
            <xsl:when test="$class='Link'">
				<xsl:attribute name="color">#0000FF</xsl:attribute>
            </xsl:when>
            <xsl:otherwise>
<!--				<xsl:attribute name="font-size">10pt</xsl:attribute>
				<xsl:attribute name="color">#000000</xsl:attribute>
-->            </xsl:otherwise>
        </xsl:choose>
	</xsl:template>
</xsl:stylesheet>