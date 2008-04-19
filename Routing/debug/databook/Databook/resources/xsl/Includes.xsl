<?xml version="1.0" encoding="ISO-8859-1"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:xd="http://www.pnp-software.com/XSLTdoc">
  
  <xd:doc type="stylesheet">
    <xd:author>João Daniel Togni</xd:author>
    <xd:MyProperty>
      <xd:MyCaption>Maintained by:</xd:MyCaption>
      <xd:MyLine>Rodrigo Gasparoni Santos</xd:MyLine>
    </xd:MyProperty>
    <xd:copyright>&#169; 2006, Nangate Inc. All rights reserved.</xd:copyright>
    <xd:MyProperty>
      <xd:MyCaption>Copyright Notice:</xd:MyCaption>
      <xd:MyLine>This document contains confidential and proprietary information.</xd:MyLine>
      <xd:MyLine>Reproduction or usage of this document, in part or whole, by any means, electrical, mechanical, optical, chemical or otherwise is prohibited, without written permission from Nangate Inc.</xd:MyLine>
      <xd:MyLine>The information contained herein is protected by Danish and international copyright laws.</xd:MyLine>
    </xd:MyProperty>
    <xd:cvsId>$Id: Includes.xsl,v 1.5 2008/01/15 18:52:59 rgs Exp $</xd:cvsId>
    <xd:short>Includes package</xd:short>
    <xd:detail>This file defines a series of variables to be used by the other scripts. The values of the variables are extracted from an external configuration file (<i>config.xml</i>)</xd:detail>
  </xd:doc>
  
  
  
  <!-- load configuration xml -->
  <xsl:variable name="Config"  select="document('../../config.xml')/config"/>
  <xsl:variable name="MapNode" select="$Config/mapping"/>
  
  <xsl:variable name="Extensions" select="document('../../extensions.xml')/Extensions"/>
  
  <!-- use Html or PDF in type to switch between styles -->
  <xsl:variable name="type"><xsl:value-of select="$Config/@style"/></xsl:variable>
  
  <!-- use PDF in DocType to generate PDF -->
  <xsl:variable name="DocType"><xsl:value-of select="$Config/@doctype"/></xsl:variable>
  
  <!-- Global variables: used to locate/process fixed sections -->
  <xsl:variable name="AbsDeviation"  select="$Config/mapping/entry[@key='AbsDeviation']/@value"/>
  <xsl:variable name="DevMode"       select="'deviation'"/>
  <xsl:variable name="DevModeAbs"    select="'deviation_abs'"/>
  <xsl:variable name="DevModeBest"   select="'best'"/>
  <xsl:variable name="DevModeWorst"  select="'worst'"/>
  <xsl:variable name="FOOS"          select="'FOOS'"/>
  <xsl:variable name="FOPD"          select="'FOPD'"/>
  <xsl:variable name="FP"            select="'FP'"/>
  <xsl:variable name="MS"            select="'MS'"/>
  <xsl:variable name="NoCorner"      select="$Config/mapping/entry[@key='NoCorner']/@value"/>
  <xsl:variable name="OS"            select="'OS'"/>
  <xsl:variable name="OSRatio"       select="'OSRatio'"/>
  <xsl:variable name="PD"            select="'PD'"/>
  <xsl:variable name="PDRatio"       select="'PDRatio'"/>
  <xsl:variable name="PWR"           select="'PWR'"/>
  <xsl:variable name="RP"            select="'RP'"/>
  <xsl:variable name="RRMS"          select="'RRMS'"/>
  <xsl:variable name="ST"            select="'ST'"/>
  <xsl:variable name="HT"            select="'HT'"/>
  <xsl:variable name="RC"            select="'RC'"/>
  <xsl:variable name="RM"            select="'RM'"/>
  <xsl:variable name="FO4D"          select="'FO4D'"/>
  <xsl:variable name="RelDeviation"  select="$Config/mapping/entry[@key='RelDeviation']/@value"/>
  <xsl:variable name="XmlCornerList" select="document('../../CornerList.xml')"/>
  <xsl:variable name="XmlLibrary"    select="document(concat('../../',/*/@LibFile,/Library/FileName))"/>
  <xsl:variable name="CAP"          select="'CAP'"/>
  
  <xsl:variable name="LibMode">
    
    <xsl:variable name="CornerName" select="/*/@CornerName"/>
    <xsl:variable name="CornerNode" select="$XmlCornerList/CornerList/Corner[@Name=$CornerName]"/>
    
    <xsl:choose>
      <xsl:when test="$CornerNode/@Type='abs'">
      <xsl:value-of select="$DevModeAbs"/>
      </xsl:when>
      <xsl:when test="$CornerNode/@Type='rel'">
      <xsl:value-of select="$DevMode"/>
      </xsl:when>
      <xsl:when test="$CornerNode/@Type='best'">
      <xsl:value-of select="$DevModeBest"/>
      </xsl:when>
      <xsl:when test="$CornerNode/@Type='worst'">
      <xsl:value-of select="$DevModeWorst"/>
      </xsl:when>
      <xsl:otherwise>normal</xsl:otherwise>
    </xsl:choose>
    
  </xsl:variable>
  
  <xsl:variable name="RootPath">
    <xsl:choose>
      <xsl:when test="/Cell">..</xsl:when>
      <xsl:when test="/Profilings">..</xsl:when>
      <xsl:otherwise>.</xsl:otherwise>
    </xsl:choose>
  </xsl:variable> 
  
  <xsl:variable name="UnitNode" select="$Config/units[@type=$LibMode]"/>
  
  
  
  <!-- Add the stylesheets (basic and databook specific) according to $type and javascripts -->
  <xd:doc>
    <xd:short>Adds the stylesheets to the current document, according to the style specified in <i>config.xml</i>.</xd:short>
    <xd:param name="dir" type="string">specifies the current working directory</xd:param>
  </xd:doc>
  <xsl:template name="Includes">
    
    <xsl:param name="dir">.</xsl:param>
    
    <xsl:if test="$DocType!='PDF'">
      
      <link>
        <xsl:attribute name="rel">stylesheet</xsl:attribute>
        <xsl:attribute name="type">text/css</xsl:attribute>
        <xsl:attribute name="href"><xsl:value-of select="$dir"/>/resources/css/Basic.css</xsl:attribute>
      </link>
      <link>
        <xsl:attribute name="rel">stylesheet</xsl:attribute>
        <xsl:attribute name="type">text/css</xsl:attribute>
        <xsl:attribute name="href"><xsl:value-of select="$dir"/>/resources/css/CellLib.css</xsl:attribute>
      </link>
      <link>
        <xsl:attribute name="rel">stylesheet</xsl:attribute>
        <xsl:attribute name="type">text/css</xsl:attribute>
        <xsl:attribute name="href"><xsl:value-of select="$dir"/>/resources/css/Basic<xsl:value-of select="$type"/>.css</xsl:attribute>
      </link>
      <link>
        <xsl:attribute name="rel">stylesheet</xsl:attribute>
        <xsl:attribute name="type">text/css</xsl:attribute>
        <xsl:attribute name="href"><xsl:value-of select="$dir"/>/resources/css/CellLib<xsl:value-of select="$type"/>.css</xsl:attribute>
      </link>
      
      <script type="text/javascript">
        var openImg = "<xsl:value-of select="$dir"/>/resources/image/open_square.gif";
        var closeImg = "<xsl:value-of select="$dir"/>/resources/image/close_square.gif";
      </script>
      <script type="text/javascript">
        <xsl:attribute name="src"><xsl:value-of select="$dir"/>/resources/js/main.js</xsl:attribute>
      </script>
      <script>
        function hidestatus(){
        window.status='';
        return true;
        }
        document.onmouseover=hidestatus;
        document.onmouseout=hidestatus;
      </script>
      
    </xsl:if>
    
  </xsl:template>

</xsl:stylesheet>
