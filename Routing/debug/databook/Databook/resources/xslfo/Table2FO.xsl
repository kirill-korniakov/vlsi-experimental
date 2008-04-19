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
	
	<!-- Create tables in pdf -->    
	
    <xsl:template match="table">
        <xsl:if test="not(@class) or @class!='navigation'">
            <fo:block>
                <fo:table space-after="0mm" space-before="5mm" table-layout="fixed">
                    <xsl:if test="@class='big'">
                        <xsl:attribute name="width">100%</xsl:attribute>
                    </xsl:if>
                    <xsl:apply-templates select="colgroup"/>
					<fo:table-header>
						<xsl:apply-templates select="thead/*[name()!='colgroup']"/>
					</fo:table-header>
                    <fo:table-body>
                        <xsl:apply-templates select="*[name()!='colgroup' and name()!='thead']"/>
                    </fo:table-body>
                </fo:table>
            </fo:block>
        </xsl:if>
    </xsl:template>
	
    <xsl:template match="colgroup/col">
         <xsl:choose>
            <xsl:when test="../../@class='navigation'">
                <fo:table-column/>
            </xsl:when>
            <xsl:when test="../../@class='big'">
                <fo:table-column>
				<xsl:if test="position()=1">
					<xsl:attribute name="column-width"><xsl:value-of select="$Config/pdf/table_header_width"/></xsl:attribute>
				</xsl:if>
				</fo:table-column>
            </xsl:when>
            <xsl:otherwise>
                <fo:table-column column-width="30mm"/>
            </xsl:otherwise>
        </xsl:choose>
    </xsl:template>
    
    <xsl:template match="tr">
        <fo:table-row><xsl:apply-templates select="*"/></fo:table-row>
    </xsl:template>
    
    <xsl:template match="td|th">
        <xsl:element name="fo:table-cell">
            <xsl:if test="@colspan">
                <xsl:attribute name="number-columns-spanned">
                    <xsl:value-of select="@colspan"/>
                </xsl:attribute>
            </xsl:if>
            <xsl:if test="@rowspan">
                <xsl:attribute name="number-rows-spanned">
                    <xsl:value-of select="@rowspan"/>
                </xsl:attribute>
            </xsl:if>
            <xsl:if test="@class='num'">
                 <xsl:attribute name="text-align">right</xsl:attribute>
            </xsl:if>
            <xsl:choose>
	            <xsl:when test="../../@class and ../../@class='navigation'">
		        </xsl:when>
			    <xsl:otherwise>
				    <xsl:attribute name="border-color"><xsl:value-of select="$Config/pdf/table_border_color"/></xsl:attribute>
	                <xsl:attribute name="border-width">0.5pt</xsl:attribute>
	                <xsl:attribute name="border-style">solid</xsl:attribute>
	                <xsl:if test="name()='th'">
		                <xsl:attribute name="background-color">
							<xsl:value-of select="$Config/pdf/th_bk_color"/></xsl:attribute>
	                    <xsl:attribute name="text-align">center</xsl:attribute>
	                    <xsl:attribute name="font-weight">bold</xsl:attribute>
		            </xsl:if>
	            </xsl:otherwise>
            </xsl:choose>
            <fo:block font-size="8pt">
				<xsl:call-template name="CreateFontProperties">
					<xsl:with-param name="class" select="@class"/>
				</xsl:call-template>
				<xsl:choose>
		            <xsl:when test="name()='th' and a">
		                <xsl:apply-templates/>
				    </xsl:when>
					<xsl:otherwise>
		                <xsl:apply-templates/>
					</xsl:otherwise>
				</xsl:choose>
             </fo:block>
        </xsl:element>
    </xsl:template>
	
</xsl:stylesheet>