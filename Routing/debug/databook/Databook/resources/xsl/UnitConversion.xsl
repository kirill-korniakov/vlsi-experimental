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
    <xd:cvsId>$Id: UnitConversion.xsl,v 1.1 2007/01/19 13:15:37 rgs Exp $</xd:cvsId>
    <xd:short>Numeric conversion routines</xd:short>
    <xd:detail>Converts numeric data between the units defined by the <a href="http://physics.nist.gov/cuu/Units/">International System of Units</a>, excluding the notations for <i>hecto</i> (h), <i>deka</i> (da), <i>deci</i> (d) and <i>centi</i> (c).</xd:detail>
  </xd:doc>
  
  
  
  <!-- Unit Conversion Utility -->
  <xd:doc>
    Main unit conversion template. Receives as parameter a numeric value and converts it from its original unit to the given target unit.
    <xd:param name="value" type="double">value to be converted</xd:param>
    <xd:param name="unit_node" type="element">a XML element whose attributes contain information related to <i>value</i></xd:param>
    <xd:param name="source_unit" type="string">
      <xd:short>contains the symbol for the unit in which <i>value</i> is originally represented.</xd:short>
      <xd:detail>Unless otherwise stated, it will be extracted from the <i>src</i> attribute of the <i>unit_node</i> parameter</xd:detail>
    </xd:param>
    <xd:param name="target_unit" type="string">
      <xd:short>contains the symbol for the unit to which <i>value</i> should be converted.</xd:short>
      <xd:detail>Unless otherwise stated, it will be extracted from the <i>tgt</i> attribute of the <i>unit_node</i> parameter</xd:detail>
    </xd:param>
    <xd:param name="pattern" type="string">optional parameter. May be used to specify a pattern (such as "0.000") under which the result of the conversion should be formatted</xd:param>
  </xd:doc>
  <xsl:template name="ConvertUnit">
  
    <xsl:param name="value"/>
    <xsl:param name="unit_node"/>
    <xsl:param name="header" select="'no'"/>
    <xsl:param name="source_unit" select="$unit_node/@src"/>
    <xsl:param name="target_unit" select="$unit_node/@tgt"/>
    <xsl:param name="pattern">
      <xsl:choose>
        <xsl:when test="$unit_node and $unit_node/@pattern"><xsl:value-of select="$unit_node/@pattern"/></xsl:when>
        <xsl:otherwise>0.0000</xsl:otherwise>
      </xsl:choose>
    </xsl:param>
    
    <xsl:variable name="fixedValue">
      <xsl:choose>
        <xsl:when test="/*/@CornerName=$RelDeviation and $header='no'">
          <xsl:value-of select="100 * $value"/>
        </xsl:when>
        <xsl:otherwise>
          <xsl:value-of select="$value"/>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>

    <xsl:choose>
      <xsl:when test="$Config/unit_conv/@use and $Config/unit_conv/@use='no'"><xsl:value-of select="$fixedValue"/></xsl:when>
      <xsl:when test="contains(concat('',number($fixedValue)),'NaN')"><xsl:value-of select="$fixedValue"/></xsl:when>
      <xsl:otherwise>
      
        <xsl:variable name="a">
          <xsl:call-template name="RetrieveUnit">
            <xsl:with-param name="unit" select="$source_unit"/>
          </xsl:call-template>
        </xsl:variable>
        
        <xsl:variable name="b">
          <xsl:call-template name="RetrieveUnitComplement">
            <xsl:with-param name="unit" select="$target_unit"/>
          </xsl:call-template>
        </xsl:variable>
        
        <xsl:value-of select="format-number($a*$b*$fixedValue, $pattern)"/>

      </xsl:otherwise>
    </xsl:choose>

  </xsl:template>
  
  
  
  <!-- Unit Conversion Utility Helper -->
  <xd:doc>
    Auxiliar template. Returns a number to be used as a multiplier in the ConvertUnit template.
    <xd:param name="unit" type="string">symbol for the unit whose multiplier is being queried</xd:param>
  </xd:doc>
  <xsl:template name="RetrieveUnit">
    <xsl:param name="unit"/>
    <xsl:choose>
      <xsl:when test="$unit='Y'">1000000000000000000000000</xsl:when>
      <xsl:when test="$unit='Z'">1000000000000000000000</xsl:when>
      <xsl:when test="$unit='E'">1000000000000000000</xsl:when>
      <xsl:when test="$unit='P'">1000000000000000</xsl:when>
      <xsl:when test="$unit='T'">1000000000000</xsl:when>
      <xsl:when test="$unit='G'">1000000000</xsl:when>
      <xsl:when test="$unit='M'">1000000</xsl:when>
      <xsl:when test="$unit='k'">1000</xsl:when>
  <!--<xsl:when test="$unit='h'">100</xsl:when>
      <xsl:when test="$unit='da'">10</xsl:when>
      <xsl:when test="$unit='d'">0.1</xsl:when>
      <xsl:when test="$unit='c'">0.01</xsl:when>-->
      <xsl:when test="$unit='m'">0.001</xsl:when>
      <xsl:when test="$unit='u'">0.000001</xsl:when>
      <xsl:when test="$unit='n'">0.000000001</xsl:when>
      <xsl:when test="$unit='p'">0.000000000001</xsl:when>
      <xsl:when test="$unit='f'">0.000000000000001</xsl:when>
      <xsl:when test="$unit='a'">0.000000000000000001</xsl:when>
      <xsl:when test="$unit='z'">0.000000000000000000001</xsl:when>
      <xsl:when test="$unit='y'">0.000000000000000000000001</xsl:when>
      <xsl:otherwise>1</xsl:otherwise>
    </xsl:choose>
  </xsl:template>
  
  
  
  <!-- Unit Conversion Utility Helper -->
  <xd:doc>
    Auxiliar template. Returns a number to be used as a multiplier in the ConvertUnit template. The returned value is the reciprocal (1/x) of that returned by the RetrieveUnit template.
    <xd:param name="unit" type="string">symbol for the unit whose reciprocated multiplier is being queried</xd:param>
  </xd:doc>
  <xsl:template name="RetrieveUnitComplement">
    <xsl:param name="unit"/>
    <xsl:choose>
      <xsl:when test="$unit='Y'">0.000000000000000000000001</xsl:when>
      <xsl:when test="$unit='Z'">0.000000000000000000001</xsl:when>
      <xsl:when test="$unit='E'">0.000000000000000001</xsl:when>
      <xsl:when test="$unit='P'">0.000000000000001</xsl:when>
      <xsl:when test="$unit='T'">0.000000000001</xsl:when>
      <xsl:when test="$unit='G'">0.000000001</xsl:when>
      <xsl:when test="$unit='M'">0.000001</xsl:when>
      <xsl:when test="$unit='k'">0.001</xsl:when>
  <!--<xsl:when test="$unit='h'">0.01</xsl:when>
      <xsl:when test="$unit='da'">0.1</xsl:when>
      <xsl:when test="$unit='d'">10</xsl:when>
      <xsl:when test="$unit='c'">100</xsl:when>-->
      <xsl:when test="$unit='m'">1000</xsl:when>
      <xsl:when test="$unit='u'">1000000</xsl:when>
      <xsl:when test="$unit='n'">1000000000</xsl:when>
      <xsl:when test="$unit='p'">1000000000000</xsl:when>
      <xsl:when test="$unit='f'">1000000000000000</xsl:when>
      <xsl:when test="$unit='a'">1000000000000000000</xsl:when>
      <xsl:when test="$unit='z'">1000000000000000000000</xsl:when>
      <xsl:when test="$unit='y'">1000000000000000000000000</xsl:when>
      <xsl:otherwise>1</xsl:otherwise>
    </xsl:choose>  
  </xsl:template>

</xsl:stylesheet>
