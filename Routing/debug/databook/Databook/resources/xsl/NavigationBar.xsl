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
    <xd:cvsId>$Id: NavigationBar.xsl,v 1.4 2008/01/08 11:20:45 rgs Exp $</xd:cvsId>
    <xd:short>Navigation bar builder</xd:short>
    <xd:detail>This file contains templates for building page headers and navigation bars</xd:detail>
  </xd:doc>
  
  
  
  <!-- Create header with Nangate and client logos -->
  <xd:doc>
    Builds the page header (with the title and the logos)
    <xd:param name="dir" type="string">current working directory</xd:param>
    <xd:param name="Title" type="string">title to be used for the page</xd:param>
  </xd:doc>
  <xsl:template name="NangateHeader">
    
    <xsl:param name="dir"/>
    <xsl:param name="Title"/>
    
    <xsl:if test="$DocType!='PDF'">
      <table class="navigation" width="100%">
        <colgroup><col/><col/></colgroup>
        <tr class="navigation" colspan="2" align="center"></tr>
        <tr>
          <td align="left">
            <a href="http://www.nangate.com" target="_blank">
              <xsl:element name="img">
                <xsl:attribute name="src"><xsl:value-of select="$dir"/>/resources/image/NangateLogo.gif</xsl:attribute>
                <xsl:attribute name="border">0</xsl:attribute>
              </xsl:element>
            </a>                    
          </td>
          <td align="center">
            <p class="Title"><xsl:value-of select="$Title"/></p>
          </td>
          <td align="right">
            <xsl:element name="img">
              <xsl:attribute name="src"><xsl:value-of select="$dir"/>/resources/image/<xsl:value-of select="document('../../CornerList.xml')/CornerList/Logo"/></xsl:attribute>
              <xsl:attribute name="border">0</xsl:attribute>
            </xsl:element>
          </td>
        </tr>
      </table>
    </xsl:if>
    
  </xsl:template>
  
  
  
  <!-- Build Navigation Bar (with some assumptions)-->
  <xd:doc>
    Simple navigation bar builder. Calls the BuildNavigationBar template.
    <xd:param name="Pos" type="string">defines the type of bar to be built ("Top", "Bottom"). Default value is "Top"</xd:param>
    <xd:param name="PrevName" type="string">name of the previous page. Unless otherwise stated, it will be extracted from a <i>Navigation</i> element in the original XML file</xd:param>
    <xd:param name="NextName" type="string">name of the next page. Unless otherwise stated, it will be extracted from a <i>Navigation</i> element in the original XML file</xd:param>
  </xd:doc>
  <xsl:template name="AutoBuildNavigationBar">
    
    <xsl:param name="Pos">Top</xsl:param>
    <xsl:param name="PrevName">
      <xsl:choose>
        <xsl:when test="Navigation/Previous/@isProfiling='yes'">
          <xsl:for-each select="Navigation/Previous">
            <xsl:call-template name="buildProfilingTitle"/>
          </xsl:for-each>
        </xsl:when>
        <xsl:otherwise>
          <xsl:value-of select="Navigation/Previous/@Name"/>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:param>
    <xsl:param name="NextName">
      <xsl:choose>
        <xsl:when test="Navigation/Next/@isProfiling='yes'">
          <xsl:for-each select="Navigation/Next">
            <xsl:call-template name="buildProfilingTitle"/>
          </xsl:for-each>
        </xsl:when>
        <xsl:otherwise>
          <xsl:value-of select="Navigation/Next/@Name"/>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:param>
    
    <xsl:call-template name="BuildNavigationBar">
      
      <xsl:with-param name="ParentName" select="Navigation/Parent/@Name"/>
      <xsl:with-param name="ParentLink" select="Navigation/Parent/@Link"/>
      <xsl:with-param name="PrevName" select="$PrevName"/>
      <xsl:with-param name="PrevLink" select="Navigation/Previous/@Link"/>
      <xsl:with-param name="NextName" select="$NextName"/>
      <xsl:with-param name="NextLink" select="Navigation/Next/@Link"/>
      
      <xsl:with-param name="classT">
        <xsl:choose>
          <xsl:when test="$Pos='Top'">navigation_tt</xsl:when>
          <xsl:otherwise>navigation_bt</xsl:otherwise>
        </xsl:choose>
      </xsl:with-param>
      
      <xsl:with-param name="classB">
        <xsl:choose>
          <xsl:when test="$Pos='Top'">navigation_tb</xsl:when>
          <xsl:otherwise>navigation_bb</xsl:otherwise>
        </xsl:choose>
      </xsl:with-param>
      
    </xsl:call-template>
    
  </xsl:template>
  
  
  
  <!-- Build Navigation Bar -->
  <xd:doc>
    Builds the navigation bars
    <xd:param name="ParentName" type="string">name of the parent page</xd:param>
    <xd:param name="ParentLink" type="string">address of the parent page</xd:param>
    <xd:param name="PrevName" type="string">name of the previous page</xd:param>
    <xd:param name="PrevLink" type="string">address of the previous page</xd:param>
    <xd:param name="NextName" type="string">name of the next page</xd:param>
    <xd:param name="NextLink" type="string">address of the next page</xd:param>
    <xd:param name="classT" type="string">CSS style to be applied to the top navigation bar ("navigation_tt" or "navigation_bt")</xd:param>
    <xd:param name="classB" type="string">CSS style to be applied to the bottom navigation bar ("navigation_tb" or "navigation_bb")</xd:param>
  </xd:doc>
  <xsl:template name="BuildNavigationBar">

    <xsl:param name="ParentName"/>
    <xsl:param name="ParentLink"/>
    <xsl:param name="PrevName"/>
    <xsl:param name="PrevLink"/>
    <xsl:param name="NextName"/>
    <xsl:param name="NextLink"/>
    <xsl:param name="classT"/>
    <xsl:param name="classB"/>

    <xsl:if test="$DocType!='PDF'">
    
      <hr><xsl:attribute name="class"><xsl:value-of select="$classT"/></xsl:attribute></hr>
      <table width="100%" summary="Navigation header" class="navigation">
        <tr>
        
          <td width="33%" align="left">
            <xsl:if test="$PrevName!=''">
              <xsl:call-template name="BuildNavigationBarItem">
                <xsl:with-param name="Name" select="$PrevName"/>
                <xsl:with-param name="Link" select="$PrevLink"/>
                <xsl:with-param name="Title">Previous</xsl:with-param>
              </xsl:call-template>
            </xsl:if>
          </td>
          
          <td width="33%" align="center">
            <xsl:call-template name="BuildNavigationBarItem">
              <xsl:with-param name="Name" select="$ParentName"/>
              <xsl:with-param name="Link" select="$ParentLink"/>
              <xsl:with-param name="Title">Up</xsl:with-param>
            </xsl:call-template>
          </td>
          
          <td width="33%" align="right">
            <xsl:if test="$NextName!=''">
              <xsl:call-template name="BuildNavigationBarItem">
                <xsl:with-param name="Name" select="$NextName"/>
                <xsl:with-param name="Link" select="$NextLink"/>
                <xsl:with-param name="Title">Next</xsl:with-param>
              </xsl:call-template>
            </xsl:if>
          </td>
          
        </tr>
      </table>
      
      <hr><xsl:attribute name="class"><xsl:value-of select="$classB"/></xsl:attribute></hr>
      <table class="navigation" width="100%">
        <colgroup><col/><col/></colgroup>
        <tr>
          <td align="left">
            <p class="copyright">Databook Build Date: <xsl:value-of select="$XmlCornerList/CornerList/Date"/></p>
          </td>
          <td align="right">
            <p class="copyright">Copyright © 2004-2008 Nangate Inc.</p>
          </td>
        </tr>
      </table>
      
    </xsl:if>
    
  </xsl:template>
  
  
  
  <!-- Build Navigation Bar Items (prev, next, up) -->
  <xd:doc>
    Builds an individual link in the navigation bar
    <xd:param name="Name" type="string">name of the page where the link points to</xd:param>
    <xd:param name="Link" type="string">address of the page the link points to</xd:param>
    <xd:param name="Title" type="string">title of the link ("Previous", "Next", "Up")</xd:param>
  </xd:doc>
  <xsl:template name="BuildNavigationBarItem">
    
    <xsl:param name="Name"/>
    <xsl:param name="Link"/>
    <xsl:param name="Title"/>

    <xsl:if test="$Name">
      <xsl:element name="a">
        <xsl:attribute name="href"><xsl:value-of select="$Link"/></xsl:attribute>
        <xsl:value-of select="$Title"/> (<xsl:value-of select="normalize-space($Name)"/>)
      </xsl:element>
    </xsl:if>
    
  </xsl:template>
  
</xsl:stylesheet>
