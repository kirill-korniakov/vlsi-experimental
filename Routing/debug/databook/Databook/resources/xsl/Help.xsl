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
  
  Maintained by:
  Rodrigo Gasparoni Santos
  -->
  
  <xsl:include href="Utils.xsl"/>
  <xsl:include href="Includes.xsl"/>
  <xsl:include href="NavigationBar.xsl"/>
  
  <!-- Cell -->
  <xsl:template match="/">
    
    <xsl:variable name="Title" select="title"/>    
    <html>
      <head>
        <title>
          <xsl:value-of select="$Title"/>
        </title>
        <xsl:call-template name="Includes">
          <xsl:with-param name="dir" select="'../..'"/>
        </xsl:call-template>
      </head>
      <body onLoad="NanInit();"> 
        
        <xsl:call-template name="NangateHeader">
          <xsl:with-param name="dir" select="'../..'"/>
          <xsl:with-param name="Title" select="$Title"/>
        </xsl:call-template>
        
        <hr class="navigation"/>
        
        <table class="navigation" width="100%">
          <colgroup><col/><col/></colgroup>
          <tr>
            <td align="left">
              <p class="copyright">Help</p>
            </td>
            <td align="right">
              <p class="copyright">Copyright © 2004-2008 Nangate Inc.</p>
            </td>
          </tr>
        </table>
        
        <!-- Help contents -->
        <xsl:apply-templates select="help"/>

        <hr class="navigation"/>
        
        <table class="navigation" width="100%">
          <colgroup><col/><col/></colgroup>
          <tr>
          <td align="left">
            <p class="copyright">Help</p>
          </td>
          <td align="right">
            <p class="copyright">Copyright © 2004-2008 Nangate Inc.</p>
          </td>
          </tr>
        </table>
        
      </body>
    </html>
    
  </xsl:template>
  
  
  
  <xsl:template match="help">
    
    <xsl:if test=".!=/help">
      
      <p>
        <xsl:attribute name="class">
          <xsl:choose>
            <xsl:when test="not(../..)">Title</xsl:when>
            <xsl:when test="not(../../..)">Caption</xsl:when>
            <xsl:when test="not(../../../..)">SubCaption</xsl:when>
            <xsl:otherwise>error</xsl:otherwise>
          </xsl:choose>
        </xsl:attribute>
        <xsl:value-of select="title"/>
      </p>
      
      <ul>
        <li>
          <xsl:apply-templates select="*[name()!='title']"/>
        </li>
      </ul>
      
    </xsl:if>
    
    <xsl:if test=".=/help">
      <xsl:apply-templates select="*[name()!='title']"/>
    </xsl:if>
    
  </xsl:template>
  
  
  
  <xsl:template match="desc|liberty">
    <p>
      <span class="bold">
        <xsl:choose>
          <xsl:when test="name()='desc'">Description</xsl:when>
          <xsl:otherwise>Liberty Property</xsl:otherwise>
        </xsl:choose>: 
      </span>
      <xsl:copy-of select="."/>
    </p>
  </xsl:template>
  
</xsl:stylesheet>
