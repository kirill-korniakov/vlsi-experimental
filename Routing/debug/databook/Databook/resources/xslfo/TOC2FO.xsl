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
	<!-- Create PDF outline, must be commented out in fop beta version -->	
    <xsl:template name="CreateOutline">
        <xsl:param name="toc" 
            select="/all/html[position()=$TOCPos]/body/ol|/all/html[position()=$TOCPos]/body/ul"/>
		<xsl:for-each select="$toc/li">
			<xsl:variable name="val">
				<xsl:choose>
					<xsl:when test="a/@href='Corner [NoCorner]'">
						<xsl:value-of select="/all/html[position()=$TOCPos]/head/title"/>
					</xsl:when>
					<xsl:when test="a">
						<xsl:value-of select="a/text()|a/p/text()"/>
					</xsl:when>
					<xsl:otherwise>
						<xsl:value-of select="text()"/>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:variable>
			<fox:outline>
				<xsl:attribute name="internal-destination">
					<xsl:choose>
						<xsl:when test="a">
							<xsl:value-of select="a/@href"/>
						</xsl:when>
						<xsl:otherwise>
							<xsl:value-of select="$val"/>
						</xsl:otherwise>
					</xsl:choose>
				</xsl:attribute>
				<fox:label>
					<xsl:value-of select="$val"/>
				</fox:label>
				<xsl:call-template name="CreateOutline">
					<xsl:with-param name="toc" select="span/ol|span/ul|ol|ul"/>
				</xsl:call-template>
			</fox:outline> 
		</xsl:for-each>
    </xsl:template>
                

 	<!-- process all TOC related items -->	
    <xsl:template match="ol|ul">
		<xsl:choose>	   
			<xsl:when test="@class='TOC1' or @class='TOC2' or @class='TOC3' or @class='TOC4'">
	            <fo:block margin-left="0.3cm">
	                <fo:table table-layout="fixed" width="100%" space-before="3mm" space-after="0mm">
		                <fo:table-column/>
			            <fo:table-body>
				            <fo:table-row>
					            <fo:table-cell>
						            <fo:block><xsl:apply-templates/></fo:block>
	                            </fo:table-cell>
		                    </fo:table-row>
			            </fo:table-body>
				    </fo:table>
		        </fo:block>
        </xsl:when>
		<xsl:when test="@class=''">
		    <fo:list-block> Œ
				<xsl:apply-templates/>				 
		    </fo:list-block>
		</xsl:when>
	    <xsl:otherwise>
			 <xsl:apply-templates/>
		</xsl:otherwise>
	   </xsl:choose>
    </xsl:template>
    
	<!-- process all TOC related items -->	
    <xsl:template match="li">
		<xsl:variable name="class" select="../@class"/>
        <xsl:choose>
			<xsl:when test="$class='TOC1' or $class='TOC2' or $class='TOC3' or $class='TOC4'">
	
				<fo:block space-before="0pt">
					<xsl:call-template name="CreateFontProperties">
						<xsl:with-param name="class" select="$class"/>
					</xsl:call-template>
					<xsl:attribute name="margin-left">
				        <xsl:choose>
				            <xsl:when test="$class='TOC1'">0pt</xsl:when>
				            <xsl:when test="$class='TOC2'">10pt</xsl:when>
				            <xsl:when test="$class='TOC3'">20pt</xsl:when>
				            <xsl:when test="$class='TOC4'">30pt</xsl:when>
				            <xsl:otherwise>0pt</xsl:otherwise>
				        </xsl:choose></xsl:attribute>
					<xsl:attribute name="font-size">
					    <xsl:choose>
				            <xsl:when test="$class='TOC1'">18pt</xsl:when>
				            <xsl:when test="$class='TOC2'">16pt</xsl:when>
				            <xsl:when test="$class='TOC3'">14pt</xsl:when>
				            <xsl:when test="$class='TOC4'">12pt</xsl:when>
				            <xsl:otherwise>10pt</xsl:otherwise>
				        </xsl:choose></xsl:attribute>
					<xsl:attribute name="color">
						<xsl:choose>
							<xsl:when test="$class='TOC1'"><xsl:value-of select="$Config/pdf/toc1_color"/></xsl:when>
							<xsl:when test="$class='TOC2'"><xsl:value-of select="$Config/pdf/toc2_color"/></xsl:when>
				            <xsl:when test="$class='TOC3'"><xsl:value-of select="$Config/pdf/toc3_color"/></xsl:when>
				            <xsl:when test="$class='TOC4'"><xsl:value-of select="$Config/pdf/toc4_color"/></xsl:when>
							<xsl:otherwise><xsl:value-of select="$Config/pdf/text_color"/></xsl:otherwise>
				        </xsl:choose></xsl:attribute>
	
			        <xsl:variable name="TitleName">
			            <xsl:choose>
			                <xsl:when test="a/p"><xsl:value-of select="normalize-space(a/p/text())"/></xsl:when>
			                <xsl:when test="a"><xsl:value-of select="normalize-space(a/text())"/></xsl:when>
			                <xsl:otherwise><xsl:value-of select="text()"/></xsl:otherwise>
			            </xsl:choose>
			        </xsl:variable>
			        <xsl:variable name="Title">
			            <xsl:choose>
			                <xsl:when test="$class='TOC1' or name(..)='ol'">
			                    <xsl:number count="li" format="1"/>. </xsl:when>
			                <xsl:when test="$class='TOC2'">
			                    <xsl:number level="multiple" count="li" format="1.1"/>. </xsl:when>
			                <xsl:when test="$class='TOC3'">
			                    <xsl:number level="multiple" count="li" format="1.1.1"/>. </xsl:when>
			                <xsl:when test="$class='TOC4'">
			                    <xsl:number level="multiple" count="li" format="1.1.1.1"/>. </xsl:when>
							<xsl:otherwise>* </xsl:otherwise>
			            </xsl:choose>
						<xsl:choose>
							<xsl:when test="$class='TOC1' and text()='Corner NoCorner'">
								<xsl:value-of select="/all/html[position()=$TOCPos]/head/title"/>
							</xsl:when>
							<xsl:otherwise>
						        <xsl:value-of select="$TitleName"/>
							</xsl:otherwise>
						</xsl:choose>
			        </xsl:variable>
	
					<xsl:choose>        
			            <xsl:when test="a">
			                <fo:block text-align-last="justify" end-indent="24pt" last-line-end-indent="-24pt">
			                    <fo:inline keep-with-next.within-line="always">
			                        <fo:basic-link>
			                            <xsl:attribute name="internal-destination">
			                                <xsl:value-of select="a/@href"/>
			                            </xsl:attribute>
			                            <xsl:value-of select="$Title"/>
			                        </fo:basic-link>
			                    </fo:inline>
			                    <fo:inline keep-together.within-line="always">
			                        <fo:leader leader-pattern="dots" leader-pattern-width="3pt" leader-alignment="reference-area"
			                            keep-with-next.within-line="always"/>
			                        <xsl:element name="fo:page-number-citation">
			                            <xsl:attribute name="ref-id">
			                                <xsl:value-of select="a/@href"/>
			                            </xsl:attribute>
			                        </xsl:element>
			                    </fo:inline>
			                </fo:block>
					        <xsl:apply-templates select="*[name()!='a']"/>
				        </xsl:when>
				        <xsl:otherwise>******************</xsl:otherwise>
			        </xsl:choose>
				</fo:block>
			</xsl:when>
			
			<xsl:otherwise>
		        <fo:list-item > 
		            <fo:list-item-label end-indent="label-end()" >
						<fo:block>
							<xsl:if test="not(../@class) or ../@class!='no_bullet'">
								&#x2022; 
							</xsl:if>
						</fo:block>
		            </fo:list-item-label>
				    <fo:list-item-body start-indent="body-start()" >
						<fo:block>
							<xsl:apply-templates select="*"/>
						</fo:block>
		            </fo:list-item-body>
			    </fo:list-item>
			</xsl:otherwise>
		</xsl:choose>		
    </xsl:template>
    
    
</xsl:stylesheet>    
