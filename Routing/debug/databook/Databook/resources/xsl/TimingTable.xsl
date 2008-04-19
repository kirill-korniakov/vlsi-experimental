<?xml version="1.0" encoding="ISO-8859-1"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:xd="http://www.pnp-software.com/XSLTdoc">

  <xd:doc type="stylesheet">
    <xd:author>João Daniel Togni</xd:author>
    <xd:MyProperty>
      <xd:MyCaption>Maintained by:</xd:MyCaption>
      <xd:MyLine>Rodrigo Gasparoni Santos</xd:MyLine>
      <xd:MyLine>Felipe Freitag Vargas</xd:MyLine>
    </xd:MyProperty>
    <xd:copyright>&#169; 2006, Nangate Inc. All rights reserved.</xd:copyright>
    <xd:MyProperty>
      <xd:MyCaption>Copyright Notice:</xd:MyCaption>
      <xd:MyLine>This document contains confidential and proprietary information.</xd:MyLine>
      <xd:MyLine>Reproduction or usage of this document, in part or whole, by any means, electrical, mechanical, optical, chemical or otherwise is prohibited, without written permission from Nangate Inc.</xd:MyLine>
      <xd:MyLine>The information contained herein is protected by Danish and international copyright laws.</xd:MyLine>
    </xd:MyProperty>
    <xd:cvsId>$Id: TimingTable.xsl,v 1.22 2008/01/16 15:21:02 rgs Exp $</xd:cvsId>
    <xd:short>Timing table scripts</xd:short>
    <xd:detail>Used in the cell pages to present look-up tables</xd:detail>
  </xd:doc>
  
  <xsl:variable name="Corner" select="/ReducedData/@CornerName|/FirstOrder/@CornerName|/Cell/@CornerName"/>
  
  <!--
      Overview:
      This code aims the printing of timing tables. It handles the timing tables present in the cell pages and timing tables present in the summaries.
      It is also responsible for creating all the data and behavior related with the tables: queries, tooltip, highlight and traceability.
      Those behaviors are implemented in JavaScript and called from here.
  -->

  <xd:doc> 
    Main (Wrapper) function to print timing tables (within a cell, not summary tables). 
    It basically defines few values and calls PrintTimingTable_Int. 
    <xd:param name="SecId" type="string">identifier for the section to be created.</xd:param>
    <xd:param name="table_rise" type="XMLNode">
      <xd:short>Rise table node.</xd:short>
      <xd:detail>The xml node containing the rise table.</xd:detail>
    </xd:param>
    <xd:param name="table_fall" type="XMLNode">
      <xd:short>Fall table node.</xd:short>
      <xd:detail>The xml node containing the fall table.</xd:detail>
    </xd:param>
    <xd:param name="unit_node" type="XMLNode">Unit node used to convert values and add proper labels. </xd:param>
    <xd:param name="table_label_fall" type="string">Label to be used in headers (2 first lines). Build from UnitNode if not provided.</xd:param>
    <xd:param name="table_label_rise" type="string">Label to be used in headers (2 first lines). Build from UnitNode if not provided.</xd:param>
    <xd:param name="sub_link" type="string">Not used anymore (only in Generic table, which doesnt call that function)</xd:param>
    <xd:param name="ignore_v2" type="char">Used to ignore second value when template has two (will create a header with 1 row).</xd:param>
    <xd:param name="templates_orig" type="XMLNode">Template node (used to create header).</xd:param>

    <xd:param name="internal" type="HTML">section contents</xd:param>
  </xd:doc>
  <xsl:template name="PrintTimingTable">
    
    <xsl:param name="SecId"/>
    <xsl:param name="table_rise"/>
    <xsl:param name="table_fall"/>
    <xsl:param name="unit_node"/>
    <xsl:param name="table_label_fall" select="'fall '"/>
    <xsl:param name="table_label_rise" select="'rise '"/>
    <xsl:param name="sub_link"/>
    <xsl:param name="ignore_v2" select="'n'"/>
    <xsl:param name="templates_orig" select="document(concat('../../Templates_',$Corner,'.xml'))/Templates" />
    
    <xsl:variable name="templates" select="$templates_orig"/>
    
    <xsl:choose>
      <!-- breaks down the table if templates are different -->
      <xsl:when test="$table_rise and $table_fall and $table_rise/LUT/@Template != $table_fall/LUT/@Template">
        
        <xsl:call-template name="PrintTimingTable_Int">
          <xsl:with-param name="table_rise"       select="null"/>
          <xsl:with-param name="table_fall"       select="$table_fall"/>
          <xsl:with-param name="unit_node"        select="$unit_node"/>
          <xsl:with-param name="table_label_fall" select="$table_label_fall"/>
          <xsl:with-param name="table_label_rise" select="$table_label_rise"/>
          <xsl:with-param name="sub_link"         select="$sub_link"/>
          <xsl:with-param name="templates"        select="$templates"/>
          <xsl:with-param name="SecId"            select="$SecId"/>
          <xsl:with-param name="ignore_v2"        select="$ignore_v2"/>
        </xsl:call-template>
        
        <xsl:call-template name="PrintTimingTable_Int">
          <xsl:with-param name="table_rise"       select="$table_rise"/>
          <xsl:with-param name="table_fall"       select="null"/>
          <xsl:with-param name="unit_node"        select="$unit_node"/>
          <xsl:with-param name="table_label_fall" select="$table_label_fall"/>
          <xsl:with-param name="table_label_rise" select="$table_label_rise"/>
          <xsl:with-param name="sub_link"         select="$sub_link"/>
          <xsl:with-param name="templates"        select="$templates"/>
          <xsl:with-param name="SecId"            select="$SecId"/>
          <xsl:with-param name="ignore_v2"        select="$ignore_v2"/>
        </xsl:call-template>
        
      </xsl:when>
      <xsl:otherwise>
        <xsl:call-template name="PrintTimingTable_Int">
          <xsl:with-param name="table_rise"       select="$table_rise"/>
          <xsl:with-param name="table_fall"       select="$table_fall"/>
          <xsl:with-param name="unit_node"        select="$unit_node"/>
          <xsl:with-param name="table_label_fall" select="$table_label_fall"/>
          <xsl:with-param name="table_label_rise" select="$table_label_rise"/>
          <xsl:with-param name="sub_link"         select="$sub_link"/>
          <xsl:with-param name="templates"        select="$templates"/>
          <xsl:with-param name="SecId"            select="$SecId"/>
          <xsl:with-param name="ignore_v2"        select="$ignore_v2"/>
        </xsl:call-template>
      </xsl:otherwise>
    </xsl:choose>
    
  </xsl:template>
  
  
  <xd:doc>This function implements the top level table implementation in HTML.
    <xd:param name="SecId" type="string">identifier for the section to be created.</xd:param>
    <xd:param name="table_rise" type="XMLNode">
      <xd:short>Rise table node.</xd:short>
      <xd:detail>The xml node containing the rise table.</xd:detail>
    </xd:param>
    <xd:param name="table_fall" type="XMLNode">
      <xd:short>Fall table node.</xd:short>
      <xd:detail>The xml node containing the fall table.</xd:detail>
    </xd:param>
    <xd:param name="unit_node" type="XMLNode">Unit node used to convert values and add proper labels. </xd:param>
    <xd:param name="table_label_fall" type="string">Label to be used in headers (2 first lines). Build from UnitNode if not provided.</xd:param>
    <xd:param name="table_label_rise" type="string">Label to be used in headers (2 first lines). Build from UnitNode if not provided.</xd:param>
    <xd:param name="sub_link" type="string">Not used anymore (only in Generic table, which doesnt call that function)</xd:param>
    <xd:param name="ignore_v2" type="char">Used to ignore second value when template has two (will create a header with 1 row).</xd:param>
    <xd:param name="templates" type="XMLNode">Template node (used to create header).</xd:param>

    <xd:param name="internal" type="HTML">section contents</xd:param>
  </xd:doc>
  <xsl:template name="PrintTimingTable_Int">
    
    <xsl:param name="SecId"/>
    <xsl:param name="table_rise"/>
    <xsl:param name="table_fall"/>
    <xsl:param name="unit_node"/>
    <xsl:param name="table_label_fall"/>
    <xsl:param name="table_label_rise"/>
    <xsl:param name="sub_link"/>
    <xsl:param name="templates"/>
    <xsl:param name="ignore_v2"/>
    
    <!-- if there is a rise of fall table -->
    <xsl:if test="$table_fall/LUT | $table_rise/LUT">
      
      <xsl:variable name="comboBoxLink">CB_<xsl:value-of select="$table_fall/LUT/@gid"/>_<xsl:value-of select="$table_rise/LUT/@gid"/></xsl:variable>
      
      <a>
        
        <xsl:attribute name="name">Table_<xsl:value-of select="$table_fall/LUT/@gid"/></xsl:attribute>
        
        <a>
          
          <xsl:attribute name="name">Table_<xsl:value-of select="$table_rise/LUT/@gid"/></xsl:attribute>
          
          <!-- if it is a scalar table build it differently -->
          <xsl:choose>
            <xsl:when test="$table_rise/LUT/@Template='scalar' or $table_fall/LUT/@Template='scalar'">
              <table border="1" class="big">
                
                <colgroup><col/><col/><col/></colgroup>
                
                <xsl:if test="$table_fall">
                  <tr>
                    <!-- print table header: column (scalar) -->
                    <xsl:call-template name="CreateTableHeader">
                      <xsl:with-param name="table_fall" select="$table_fall"/>
                      <xsl:with-param name="table_rise" select="$table_rise"/>
                    </xsl:call-template>
                    
                    <xsl:call-template name="FillTableValues">
                      <xsl:with-param name="table"     select="$table_fall/LUT"/>
                      <xsl:with-param name="SecId"     select="$SecId"/>
                      <xsl:with-param name="name"      select="concat($table_label_fall, $unit_node/out/@label)"/>
                      <xsl:with-param name="unit_node" select="$unit_node"/>
                    </xsl:call-template>
                    
                  </tr>
                </xsl:if>
                
                <xsl:if test="$table_rise">
                  <tr>
                    
                    <!-- if not done before, print table header: column (scalar) -->
                    <xsl:if test="not($table_fall/LUT)">
                      <xsl:call-template name="CreateTableHeader">
                        <xsl:with-param name="table_fall" select="$table_fall"/>
                        <xsl:with-param name="table_rise" select="$table_rise"/>
                      </xsl:call-template>
                    </xsl:if>
                    
                    <xsl:call-template name="FillTableValues">
                      <xsl:with-param name="table"     select="$table_rise/LUT"/>
                      <xsl:with-param name="SecId"     select="$SecId"/>
                      <xsl:with-param name="name"      select="concat($table_label_rise, $unit_node/out/@label)"/>
                      <xsl:with-param name="unit_node" select="$unit_node"/>
                    </xsl:call-template>
                    
                  </tr>
                </xsl:if>
                
                <xsl:call-template name="CreateContext">
                  <xsl:with-param name="table"    select="$table_fall|$table_rise"/>
                  <xsl:with-param name="colCount" select="3"/>
                </xsl:call-template>
              </table>
            </xsl:when>
            <xsl:otherwise>
              <!-- if it is not a scalar table -->
              <xsl:for-each select="$templates/Template[@Name = $table_fall/LUT/@Template] | $templates/Template[@Name = $table_rise/LUT/@Template]">
                <!-- this for selects the templates in order to build headers: 2 first lines -->
                <xsl:variable name="var_name_1">
                  <xsl:call-template name="Map">
                    <xsl:with-param name="map" select="$MapNode/TableHeader"/>
                    <xsl:with-param name="key" select="variable_1/Name"/>
                  </xsl:call-template>
                  <xsl:value-of select="concat(' ',$unit_node/var1/@label)"/>
                </xsl:variable>
                
                <!-- create variable 2 if present and not ignored -->
                <xsl:variable name="var_name_2">
                  <xsl:if test="variable_2 and $ignore_v2='n'">
                    <xsl:call-template name="Map">
                      <xsl:with-param name="map" select="$MapNode/TableHeader"/>
                      <xsl:with-param name="key" select="variable_2/Name"/>
                    </xsl:call-template>
                    <xsl:value-of select="concat(' ',$unit_node/var2/@label)"/>
                  </xsl:if>
                </xsl:variable>
                
                <xsl:variable name="extraCol">
                  <xsl:choose>
                    <xsl:when test="count($table_fall/LUT) > 1 or count($table_rise/LUT) > 1">1</xsl:when>
                    <xsl:otherwise>0</xsl:otherwise>
                  </xsl:choose>
                </xsl:variable>
                
                <!-- table start -->
                <table border="1" class="big">
                  
                  <xsl:if test="$table_fall/stats or $table_rise/stats">
                    <xsl:attribute name="id"><xsl:value-of select="$comboBoxLink"/></xsl:attribute>
                  </xsl:if>
                  
                  <!-- build top part of the table -->
                  <xsl:call-template name="BuildTableHeader">
                    <xsl:with-param name="var_name_1"     select="$var_name_1"/>
                    <xsl:with-param name="var_name_2"     select="$var_name_2"/>
                    <xsl:with-param name="unit_node"      select="$unit_node"/>
                    <xsl:with-param name="header_colspan" select="2 + $extraCol"/>
                    <xsl:with-param name="ignore_v2"      select="$ignore_v2"/>
                  </xsl:call-template>
                  
                  <xsl:variable name="fall_label">
                    <xsl:choose>
                      <xsl:when test="$table_fall/stats">
                        <xsl:value-of select="$table_label_fall"/><xsl:if test="$table_fall/stats/*[1]/@unit != ''">[<xsl:value-of select="$table_fall/stats/*[1]/@unit"/>]</xsl:if>
                      </xsl:when>
                      <xsl:otherwise><xsl:value-of select="concat($table_label_fall, $unit_node/out/@label)"/>
                      </xsl:otherwise>
                    </xsl:choose>
                  </xsl:variable>
                  
                  <xsl:variable name="rise_label">
                    <xsl:choose>
                      <xsl:when test="$table_rise/stats">
                        <xsl:value-of select="$table_label_rise"/><xsl:if test="$table_rise/stats/*[1]/@unit != ''">[<xsl:value-of select="$table_rise/stats/*[1]/@unit"/>]</xsl:if>
                      </xsl:when>
                      <xsl:otherwise><xsl:value-of select="concat($table_label_rise, $unit_node/out/@label)"/>
                      </xsl:otherwise>
                    </xsl:choose>
                  </xsl:variable>
                  
                  <xsl:if test="$table_fall/LUT|$table_rise/LUT">
                    
                    <xsl:if test="$table_fall/LUT">
                      <xsl:for-each select="$table_fall/LUT">
                        <!-- build fall part -->                         
                        <tr>
                          <!-- table header: first 2 cols -->
                          <xsl:if test="position() = 1">
                            <xsl:call-template name="CreateTableHeader">
                              <xsl:with-param name="table_fall" select="$table_fall"/>
                              <xsl:with-param name="table_rise" select="$table_rise"/>
                            </xsl:call-template>
                          </xsl:if>
  
                          <!-- fill other cols -->
                          <xsl:call-template name="FillTableValues">
                            <xsl:with-param name="table"     select="."/>
                            <xsl:with-param name="SecId"     select="$SecId"/>
                            <xsl:with-param name="name"      select="$fall_label"/>
                            <xsl:with-param name="unit_node" select="$unit_node"/>
                            <xsl:with-param name="extraCol"  select="$extraCol"/>
                          </xsl:call-template>
                          
                        </tr>
                      </xsl:for-each>
                    </xsl:if>
                    
                    <xsl:if test="$table_rise/LUT">
                      <xsl:for-each select="$table_rise/LUT">
                        <!-- build rise part -->
                        <tr>
                          <!-- table header: first 2 cols -->
                          <xsl:if test="not($table_fall/LUT) and position() = 1">
                            <xsl:call-template name="CreateTableHeader">
                              <xsl:with-param name="table_fall" select="$table_fall"/>
                              <xsl:with-param name="table_rise" select="$table_rise"/>
                            </xsl:call-template>
                          </xsl:if>
                          
                          <!-- fill other cols -->
                          <xsl:call-template name="FillTableValues">
                            <xsl:with-param name="table"     select="."/>
                            <xsl:with-param name="SecId"     select="$SecId"/>
                            <xsl:with-param name="name"      select="$rise_label"/>
                            <xsl:with-param name="unit_node" select="$unit_node"/>
                            <xsl:with-param name="extraCol"  select="$extraCol"/>
                          </xsl:call-template>
                          
                        </tr>
                      </xsl:for-each>
                    </xsl:if>
                    
                  </xsl:if>
                  
                  <!-- calculates the total number of cols --> 
                  <xsl:variable name="c_v2">
                    <xsl:choose>
                      <xsl:when test="variable_2 and $ignore_v2='n'">
                        <xsl:value-of select="count(variable_2/Values/Element)"/>
                      </xsl:when>
                      <xsl:otherwise>1</xsl:otherwise>
                    </xsl:choose>
                  </xsl:variable>
                  
                  <!-- create context -->
                  <xsl:call-template name="CreateContext">
                    <xsl:with-param name="table"    select="$table_fall|$table_rise"/>
                    <xsl:with-param name="colCount" select="2+$extraCol+$c_v2*count(variable_1/Values/Element)"/>
                  </xsl:call-template>
                  
                </table>
                
              </xsl:for-each>
            </xsl:otherwise>
          </xsl:choose>
          
        </a>
        
      </a>
      
      <xsl:if test="$table_fall/stats or $table_rise/stats">
        <select>
          <xsl:attribute name="onChange">updateTable(this, '<xsl:value-of select="$comboBoxLink"/>', <xsl:value-of select="count($table_fall/LUT)"/>, <xsl:value-of select="count($table_rise/LUT)"/>);</xsl:attribute>
          <xsl:choose>
            <xsl:when test="$table_fall/stats">
              <xsl:for-each select="$table_fall/stats/*">
                <option>
                  <xsl:attribute name="value"><xsl:value-of select="name()"/></xsl:attribute>
                  <xsl:attribute name="unit"><xsl:value-of select="@unit"/></xsl:attribute>
                  <xsl:value-of select="@descr"/>
                </option>
              </xsl:for-each>
            </xsl:when>
            <xsl:otherwise>
              <xsl:for-each select="$table_rise/stats/*">
                <option>
                  <xsl:attribute name="value"><xsl:value-of select="name()"/></xsl:attribute>
                  <xsl:attribute name="unit"><xsl:value-of select="@unit"/></xsl:attribute>
                  <xsl:value-of select="@descr"/>
                </option>
              </xsl:for-each>
            </xsl:otherwise>
          </xsl:choose>
        </select>
        
        <xsl:variable name="shLink">
          <xsl:choose>
            <xsl:when test="$table_fall/stats"><xsl:value-of select="$table_fall/LUT[1]/@gid"/></xsl:when>
            <xsl:otherwise><xsl:value-of select="$table_rise/LUT[1]/@gid"/></xsl:otherwise>
          </xsl:choose>
        </xsl:variable>
        
        <xsl:if test="/Cell/@ShowLocal">
          <a><xsl:attribute name="href"><xsl:value-of select="/Cell/@ShowLocal"/>#<xsl:value-of select="$shLink"/></xsl:attribute>Show local variation</a>
        </xsl:if>
        
        <xsl:if test="/Cell/@HideLocal">
          <a><xsl:attribute name="href"><xsl:value-of select="/Cell/@HideLocal"/>#<xsl:value-of select="$shLink"/></xsl:attribute>Hide local variation</a>
        </xsl:if>
        
      </xsl:if>
      
      <hr class="Title"/>
      
    </xsl:if>
    
  </xsl:template>
  
  
  
  <xd:doc>Helper to create the table header: two first columns
    <xd:param name="table_fall" type="XMLNode">Fall table node.</xd:param>
    <xd:param name="table_rise" type="XMLNode">Rise table node.</xd:param>
  </xd:doc>

  <xsl:template name="CreateTableHeader">
    
    <xsl:param name="table_fall"/>
    <xsl:param name="table_rise"/>
    
    <th>
      
      <xsl:attribute name="rowspan">
        <xsl:value-of select="count($table_fall/LUT) + count($table_rise/LUT)"/>
      </xsl:attribute>
      
      <xsl:choose>
        <xsl:when test="$table_fall">
          <xsl:call-template name="CreateRelatedPinStr">
            <xsl:with-param name="table" select="$table_fall"/>
          </xsl:call-template>
        </xsl:when>
        <xsl:otherwise>
          <xsl:call-template name="CreateRelatedPinStr">
            <xsl:with-param name="table" select="$table_rise"/>
          </xsl:call-template>
        </xsl:otherwise>
      </xsl:choose>
      
    </th>
    
  </xsl:template>
  
  
  
  <xd:doc> Helper to create the str 'frompin to topin' in table header 
    <xd:param name="table" type="XMLNode">Table node.</xd:param>
  </xd:doc>
  <xsl:template name="CreateRelatedPinStr">
    <xsl:param name="table"/>
    <!-- There are many different situations and depending on that, the string is build differently -->
    <xsl:choose>
      <xsl:when test="$table/../@RelatedPin and (name($table)='fall_transition' or name($table)='cell_fall' or name($table)='rise_transition' or name($table)='cell_rise')">
        <xsl:value-of select="$table/../@RelatedPin"/> to <xsl:value-of select="$table/../../../@Name"/>
      </xsl:when>
      <xsl:when test="$table/../@RelatedPin">
        <xsl:value-of select="$table/../../../@Name"/> to <xsl:value-of select="$table/../@RelatedPin"/>
      </xsl:when>
      <xsl:when test="(name($table)='fall_power_1D' or name($table)='rise_power_1D' or name($table)='power_1D') and $table/../@Name">
        <xsl:value-of select="$table/../@Name"/>
      </xsl:when>
      <xsl:when test="$table/../@Name">
        <xsl:value-of select="$table/../@Name"/> to <xsl:value-of select="$table/LUT/@id"/>
      </xsl:when>
    </xsl:choose>
  </xsl:template>
  
  
  
  <xd:doc> Main function to print summary tables.
    <xd:param name="SecId" type="string">identifier for the section to be created.</xd:param>
    <xd:param name="table" type="XMLNode">
      <xd:short>Table node.</xd:short>
      <xd:detail>The xml node containing the summary table.</xd:detail>
    </xd:param>
    <xd:param name="unit_node" type="XMLNode">Unit node used to convert values and add proper labels. </xd:param>
    <xd:param name="table_label" type="string">Label to be used in headers (2 first lines). Build from UnitNode if not provided.</xd:param>
    <xd:param name="sub_link" type="string">Not used anymore (only in Generic table, which doesnt call that function)</xd:param>
    <xd:param name="ignore_v2" type="char">Used to ignore second value when template has two (will create a header with 1 row).</xd:param>

    <xd:param name="internal" type="HTML">section contents</xd:param>
  </xd:doc>
  <xsl:template name="PrintTimingTable_Aglomerated">
    
    <xsl:param name="table"/>
    <xsl:param name="SecId"/>
    <xsl:param name="unit_node"/>
    <xsl:param name="sub_link"/>
    <xsl:param name="table_label">
      <xsl:value-of select="$unit_node/out/@label"/>
    </xsl:param>
    <xsl:param name="ignore_v2" select="'n'"/>
    <xsl:variable name="templates" select="document(concat('../../Templates_',$Corner,'.xml'))/Templates"/>
    
    <xsl:if test="$table/LUT">
      
      <xsl:for-each select="$table/LUT[not(preceding-sibling::LUT/@Template = @Template)]">
        <!-- selects all LUTs with the same template, thus grouping by template -->
        <xsl:variable name="Temp" select="@Template"/>
        
        <xsl:choose>
          <xsl:when test="$Temp='scalar' or (not($templates/Template[@Name = $Temp]/variable_2) and /FirstOrder)">
            <!-- Print scalar or 1 var tables for approx==scalar-->
            <table border="1" class="big">
              
              <colgroup><col/><col/><col/></colgroup>
              
              <xsl:for-each select="$table/LUT[@Template=$Temp]">
                <!-- for all the LUTs with the same template -->
                <tr>
                  
                  <th>
                    <a>
                      <!-- In case of PDF the links are created differently -->
                      <xsl:attribute name="href">
                        <xsl:choose>
                          <xsl:when test="$DocType='PDF'">
                            <xsl:value-of select="concat('@', @id,' [',$Corner,']')"/>
                          </xsl:when>
                          <xsl:otherwise>
                            <xsl:value-of select="concat('Cells/', @id, '_', $Corner, $Extensions/Cell, $sub_link)"/>
                          </xsl:otherwise>
                        </xsl:choose>
                      </xsl:attribute>
                      <xsl:value-of select="@id"/>
                    </a>
                  </th>
                  
                  <!-- print line: fill table values will create the header properly -->
                  <xsl:call-template name="FillTableValues">
                    <xsl:with-param name="table"     select="."/>
                    <xsl:with-param name="SecId"     select="$SecId"/>
                    <xsl:with-param name="name"      select="$table_label"/>
                    <xsl:with-param name="unit_node" select="$unit_node"/>
                  </xsl:call-template>
                  
                </tr>
              </xsl:for-each>
              
            </table>
          </xsl:when>
          <xsl:otherwise>
            <xsl:for-each select="$templates/Template[@Name = $Temp]">
              <!-- group LUTs per template type -->

              <!-- build table header (2 first lines) -->
              <xsl:variable name="var_name_1">
                <xsl:call-template name="Map">
                  <xsl:with-param name="map" select="$MapNode/TableHeader"/>
                  <xsl:with-param name="key" select="variable_1/Name"/>
                </xsl:call-template>
                <xsl:value-of select="concat(' ',$unit_node/var1/@label)"/>
              </xsl:variable>

              <!-- print second line if existent and not ignored -->
              <xsl:variable name="var_name_2">
                <xsl:if test="variable_2 and $ignore_v2='n'">
                  <xsl:call-template name="Map">
                    <xsl:with-param name="map" select="$MapNode/TableHeader"/>
                    <xsl:with-param name="key" select="variable_2/Name"/>
                  </xsl:call-template>
                  <xsl:value-of select="concat(' ',$unit_node/var2/@label)"/>
                </xsl:if>
              </xsl:variable>
              
              <table border="1" class="big">
                
                <!-- Prints 2D Lut's: print table header, 2 first columns -->
                <xsl:choose>
                  <xsl:when test="$table/LUT/Line">
                    <xsl:call-template name="BuildTableHeader">
                      <xsl:with-param name="var_name_1" select="$var_name_1"/>
                      <xsl:with-param name="var_name_2" select="$var_name_2"/>
                      <xsl:with-param name="unit_node"  select="$unit_node"/>
                      <xsl:with-param name="ignore_v2"  select="$ignore_v2"/>
                    </xsl:call-template>
                  </xsl:when>
                  <xsl:otherwise>
                    <xsl:call-template name="BuildTableHeader_LinApprox">
                      <xsl:with-param name="var_name_1" select="$var_name_1"/>
                      <xsl:with-param name="unit_node"  select="$unit_node"/>
                    </xsl:call-template>
                  </xsl:otherwise>
                </xsl:choose>
                
                <xsl:for-each select="$table/LUT[@Template = $Temp]">
                  <tr>
                    <th>
                      <a>
                        <!-- print link differently for pdf and html -->
                        <xsl:attribute name="href">
                          <xsl:choose>
                            <xsl:when test="$DocType='PDF'">
                              <xsl:value-of select="concat('@', @id,' [',$Corner,']')"/>
                            </xsl:when>
                            <xsl:otherwise>
                              <xsl:value-of select="concat('Cells/', @id, '_', $Corner, $Extensions/Cell, $sub_link)"/>
                            </xsl:otherwise>
                          </xsl:choose>
                        </xsl:attribute>
                        <xsl:value-of select="@id"/>
                      </a>
                    </th>
                    
                    <!-- Print table content -->
                    <xsl:call-template name="FillTableValues">
                      <xsl:with-param name="table"     select="."/>
                      <xsl:with-param name="SecId"     select="$SecId"/>
                      <xsl:with-param name="name"      select="$table_label"/>
                      <xsl:with-param name="unit_node" select="$unit_node"/>
                    </xsl:call-template>
                    
                  </tr>
                </xsl:for-each>
                
              </table>
              
            </xsl:for-each>
          </xsl:otherwise>
        </xsl:choose>
        
      </xsl:for-each>
      
      <hr class="Title"/>
      
    </xsl:if>
    
  </xsl:template>
  
  
  <xd:doc> Create plot links that are inserted in table headers.
    <xd:param name="table" type="string">Table node.</xd:param>
    <xd:param name="SecId" type="string">Section identifier.</xd:param>
  </xd:doc> 
  <xsl:template name="CreatePlotTableLink">
    
    <xsl:param name="table"/>
    <xsl:param name="SecId"/>
    
    <xsl:if test="$table/@Plot or $table/@WavePlot or $table/@EcsmWavePlot">
      
      <xsl:variable name="PlotMode">
        <xsl:choose>
          <xsl:when test="name($table/../..)='ReducedData'">summary</xsl:when>
          <xsl:when test="name($table/../..)='FirstOrder'">summary</xsl:when>
          <xsl:otherwise>timing</xsl:otherwise>
        </xsl:choose>
      </xsl:variable>
      
      <!-- search for plot name -->
      <xsl:variable name="Title">
        <xsl:call-template name="Map">
          <xsl:with-param name="map" select="$MapNode/plot/*[name()=$PlotMode]"/>
          <xsl:with-param name="key" select="$SecId"/>
        </xsl:call-template>
      </xsl:variable>
      
      <!-- build the context to be used in the plot -->
      <xsl:variable name="context">
        <xsl:choose>
          <xsl:when test="$PlotMode='timing'">

            <![CDATA[<span class="weakComment"> ]]>
            <![CDATA[<br/>Cell <span class="comment">]]>
            <xsl:choose>
              <xsl:when test="$table/../../../../../../@Name">
                <xsl:value-of select="$table/../../../../../../@Name"/>
              </xsl:when>
              <xsl:otherwise>
                <xsl:value-of select="$table/../../../../@Name"/>
              </xsl:otherwise>
            </xsl:choose>

            <![CDATA[</span><br/>Pin <span class="comment">]]>
            <xsl:call-template name="CreateRelatedPinStr">
              <xsl:with-param name="table" select="$table/.."/>
            </xsl:call-template>

            <xsl:variable name="ContextStr">
              <xsl:call-template name="CreateContextStr">
                <xsl:with-param name="table" select="$table/.."/>
              </xsl:call-template>
            </xsl:variable>

            <xsl:if test="$ContextStr!=''">
              <![CDATA[<br/>]]>
              <xsl:value-of select="$ContextStr"/>
            </xsl:if>
            <![CDATA[</span></span>]]>

          </xsl:when>
          <xsl:otherwise>
            <![CDATA[<span class="weakComment"><br/>Cell </span><span class = "comment">]]>
            <xsl:value-of select="@id"/>
            <![CDATA[</span>]]>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:variable>

      <!-- create link itself now that we have the name -->
      <xsl:if test="$table/@Plot">
        <xsl:call-template name="CreatePlotLink">
          <xsl:with-param name="link" select="$table/@Plot"/>
          <xsl:with-param name="context" select="$context"/>
          <xsl:with-param name="title">
            <![CDATA[<p class="SubTitle">]]><xsl:value-of select="$Title"/><![CDATA[</p>]]>
          </xsl:with-param>
        </xsl:call-template>
      </xsl:if>
      
      <xsl:if test="$table/@WavePlot">
        <xsl:call-template name="CreatePlotLink">
          <xsl:with-param name="link" select="$table/@WavePlot"/>
          <xsl:with-param name="context" select="$context"/>
          <xsl:with-param name="title">
            <![CDATA[<p class="SubTitle">]]>Current Waveform<![CDATA[</p>]]>
          </xsl:with-param>
          <xsl:with-param name="type">4waves</xsl:with-param>
          <xsl:with-param name="count" select="$table/@WavePlotCount"/>
        </xsl:call-template>
      </xsl:if>

      <xsl:if test="$table/@EcsmWavePlot">
        <xsl:call-template name="CreatePlotLink">
          <xsl:with-param name="link" select="$table/@EcsmWavePlot"/>
          <xsl:with-param name="context" select="$context"/>
          <xsl:with-param name="title">
            <![CDATA[<p class="SubTitle">]]>Voltage Waveform<![CDATA[</p>]]>
          </xsl:with-param>
          <xsl:with-param name="type">4waves</xsl:with-param>
          <xsl:with-param name="count" select="$table/@WavePlotCount"/>
        </xsl:call-template>
      </xsl:if>
      
      <xsl:if test="$table/@VoltWavePlot">
        <xsl:call-template name="CreatePlotLink">
          <xsl:with-param name="link" select="$table/@VoltWavePlot"/>
          <xsl:with-param name="context" select="$context"/>
          <xsl:with-param name="title">
            <![CDATA[<p class="SubTitle">]]>Voltage Waveform<![CDATA[</p>]]>
          </xsl:with-param>
          <xsl:with-param name="type">volt</xsl:with-param>
          <xsl:with-param name="count" select="$table/@WavePlotCount"/>
        </xsl:call-template>
      </xsl:if>
      
    </xsl:if>
    
  </xsl:template>
  
  
  
  <xd:doc> Table data generator: create table body and left headers. 
    <xd:param name="table" type="string">Table node.</xd:param>
    <xd:param name="SecId" type="string">Section identifier.</xd:param>
    <xd:param name="name" type="string">Header string.</xd:param>
    <xd:param name="unit_node" type="string">Unit node (used for converting the values and printing proper header).</xd:param>
    <xd:param name="extraCol" type="integer"> 0/1, indicates how many extra columns will be used due to multiple LUT elements (zero by default)</xd:param>
  </xd:doc>
  <xsl:template name="FillTableValues">
    
    <xsl:param name="table"/>
    <xsl:param name="SecId"/>
    <xsl:param name="name"/>
    <xsl:param name="unit_node"/>
    <xsl:param name="extraCol">0</xsl:param>
    
    <!-- create table internal part for all situations (ie, cell tables and summaries) -->
    <xsl:if test="$table/Line | $table/LinApprox">
      <!-- print header (first col) -->
      <xsl:if test="position() = 1 or name($table/..) = 'Table'">
        <xsl:element name="th">
        
          <xsl:if test="name($table/..) != 'Table'">
            <xsl:attribute name="rowspan">
               <xsl:value-of select="count(../LUT)"/>
            </xsl:attribute>
          </xsl:if>
          
          <xsl:if test="count(../LUT) = 1 and $extraCol = 1">
            <xsl:attribute name="colspan">2</xsl:attribute>
          </xsl:if>
          
          <!-- Behaviors are just used in HTML, never PDF, this reduces the PDF size -->
          <xsl:if test="$DocType!='PDF' and (count(../LUT) = 1 or name($table/..) = 'Table')">
            <xsl:attribute name="onclick">rowHighlight(this)</xsl:attribute>
          </xsl:if>
          
          <xsl:value-of select="$name"/><a/>
          <xsl:text> </xsl:text>
          <a>
            <xsl:attribute name="name">
              <xsl:value-of select="$table/@gid"/>
            </xsl:attribute>
          </a>
          
          <xsl:if test="count(../LUT) = 1 or name($table/..) = 'Table'">
            <xsl:call-template name="CreatePlotTableLink">
              <xsl:with-param name="table"   select="$table"/>
              <xsl:with-param name="SecId"   select="$SecId"/>
            </xsl:call-template>
          </xsl:if>
          
        </xsl:element>
      </xsl:if>
      
      <xsl:if test="count(../LUT) > 1 and name($table/..) != 'Table'">
        <th>
          <xsl:if test="$DocType!='PDF' and $extraCol = 1">
            <xsl:attribute name="onclick">rowHighlight(this)</xsl:attribute>
          </xsl:if>
          <xsl:value-of select="@id"/>
          <xsl:text> </xsl:text>
          <xsl:call-template name="CreatePlotTableLink">
            <xsl:with-param name="table"   select="$table"/>
            <xsl:with-param name="SecId"   select="$SecId"/>
          </xsl:call-template>
        </th>
      </xsl:if>
      
      <xsl:variable name="corner" select="/*/@CornerName"/>
      
      <xsl:for-each select="$table/Line | $table/LinApprox/Line">
        <!-- prepare file name for creating links -->
        <xsl:variable name="file">
          <xsl:choose>
            <xsl:when test="/FirstOrder/@Name=$FOOS">OutputSlope</xsl:when>
            <xsl:when test="/FirstOrder/@Name=$FOPD">PropagationDelay</xsl:when>
            <xsl:when test="/ReducedData/@Name=$FP">PowerFall</xsl:when>
            <xsl:when test="/ReducedData/@Name=$OSRatio">OutputSlope</xsl:when>
            <xsl:when test="/ReducedData/@Name=$PDRatio">PropagationDelay</xsl:when>
            <xsl:when test="/ReducedData/@Name=$PWR">Power</xsl:when>
            <xsl:when test="/ReducedData/@Name=$RP">PowerRise</xsl:when>
            <xsl:when test="/ReducedData/@Name=$MS">SetupHoldMetastability</xsl:when>
            <xsl:when test="/ReducedData/@Name=$RRMS">RecoveryRemovalMetastability</xsl:when>
            <xsl:when test="/ReducedData/@Name=$ST">Setup</xsl:when>
            <xsl:when test="/ReducedData/@Name=$HT">Hold</xsl:when>
            <xsl:when test="/ReducedData/@Name=$RC">Recovery</xsl:when>
            <xsl:when test="/ReducedData/@Name=$RM">Removal</xsl:when>
            <xsl:when test="/ReducedData/@Name=$FO4D">FO4Delay</xsl:when>
          </xsl:choose>
        </xsl:variable>
        
        <!-- calls proper printing function according to the table type -->
        <xsl:for-each select="Element">
          <xsl:choose>
            <xsl:when test="$XmlCornerList/CornerList/Corner[@Name=$corner]/@Ref and (/*/@Name=$OS or /*/@Name=$PD or /*/@Name=$OSRatio or /*/@Name=$PDRatio
                                                                                      or /*/@Name=$FP or /*/@Name=$RP or /*/@Name=$PWR or /*/@Name=$MS or /*/@Name=$RRMS
                                                                                       or /*/@Name=$ST or /*/@Name=$HT or /*/@Name=$RC or /*/@Name=$RM or /*/@Name=$FO4D)">
              <!-- create internals for comparison tables -->
              <xsl:call-template name="FillOneValue_Line_Ref">
                <xsl:with-param name="unit_node" select="$unit_node/out"/>
                <xsl:with-param name="corner"    select="$corner"/>
              </xsl:call-template>
            </xsl:when>
            <xsl:otherwise>
              <!-- create internals for summary tables -->
              <xsl:call-template name="FillOneValue_Line">
                <xsl:with-param name="file"      select="$file"/>
                <xsl:with-param name="unit_node" select="$unit_node/out"/>
                <xsl:with-param name="SecId" select="$SecId"/>
              </xsl:call-template>
            </xsl:otherwise>
          </xsl:choose>
        </xsl:for-each>

        <!-- create internals for linear approximation tables -->
        <xsl:if test="not($table/Line)">
          <xsl:variable name="SrcLine" select="count(preceding-sibling::*)"/>

          <xsl:variable name="SrcLut" select="../../Src/@LUT"/>

          <xsl:variable name="Sources" select="concat('elem=el_',$SrcLut,'_',$SrcLine,'_*')"/>

          <xsl:variable name="TgtLut">
            <xsl:value-of select="count(../../../preceding-sibling::Table)"/>
            <xsl:text>_</xsl:text>
            <xsl:if test="../../@gid">
              <xsl:value-of select="../../@gid"/>
            </xsl:if>
            <xsl:if test="not(../../@gid)">
              <xsl:value-of select="count(../../preceding-sibling::LUT)"/>
            </xsl:if>
          </xsl:variable>

          <xsl:call-template name="FillOneValue">

            <xsl:with-param name="TgtLut" select="$TgtLut"/>
            <xsl:with-param name="Sources" select="$Sources"/>
            <xsl:with-param name="SrcLut" select="$SrcLut"/>
            
            <xsl:with-param name="SrcFile" select="concat($file, '_', $Corner, $Extensions/Summary)"/>

            <xsl:with-param name="Text">
              <xsl:call-template name="ConvertUnit">
                <xsl:with-param name="value"     select="@cLin"/>
                <xsl:with-param name="unit_node" select="$unit_node/lin"/>
              </xsl:call-template> +
              <xsl:call-template name="ConvertUnit">
                <xsl:with-param name="value"     select="@cAng"/>
                <xsl:with-param name="unit_node" select="$unit_node/ang"/>
              </xsl:call-template> CL
            </xsl:with-param>
            
          </xsl:call-template>
          
        </xsl:if>
        
      </xsl:for-each>
      
    </xsl:if>
  </xsl:template>
  
  <xd:doc> Create table internals for comparison tables.
    <xd:param name="unit_node" type="XMLNode">Unit node (see previous references for better description)</xd:param>
    <xd:param name="corner" type="string">Corner node</xd:param>
  </xd:doc>
  <xsl:template name="FillOneValue_Line_Ref">
    
    <xsl:param name="unit_node"/>
    <xsl:param name="corner"/>
    
    <xsl:variable name="CornerNode" select="$XmlCornerList/CornerList/Corner[@Name=$corner]"/>
    <!-- prepare elements of traceability -->
    <xsl:variable name="RefNode" select="$XmlCornerList/CornerList/Corner[@Id=$CornerNode/@Ref]"/>
    <xsl:variable name="TrgNode" select="$XmlCornerList/CornerList/Corner[@Id=$CornerNode/Trg/@Id[position()=1]]"/>
    
    <xsl:variable name="file">
      <xsl:call-template name="Map">
        <xsl:with-param name="key" select="concat(/*/@Name, '_Tag')"/>
      </xsl:call-template>
    </xsl:variable>
    
    <xsl:variable name="CellName" select="../../@id"/>
    <xsl:variable name="SrcFile"  select="concat('%DIFF%', $file, '_', $RefNode/@Name, $Extensions/Summary, '%', $file, '_', $TrgNode/@Name, $Extensions/Summary, '%')"/>

    <xsl:variable name="RefSrcLut">
      <xsl:value-of select="../../Src[1]/@LUT"/>
    </xsl:variable>
    <xsl:variable name="RefSrcLine">
      <xsl:value-of select="count(../preceding-sibling::Line)"/>
    </xsl:variable>
    <xsl:variable name="RefSrcElem">
      <xsl:value-of select="count(preceding-sibling::*)"/>
    </xsl:variable>

    <xsl:variable name="RefSrcId">
      <xsl:value-of select="concat('elem=el_',$RefSrcLut,'_',$RefSrcLine,'_',$RefSrcElem)"/>
    </xsl:variable>

    <xsl:variable name="TrgSrcLut">
      <xsl:value-of select="../../Src[2]/@LUT"/>
    </xsl:variable>

    <xsl:variable name="TrgSrcId">
      <xsl:choose>
        <xsl:when test="./Src">
          <xsl:for-each select="Src">
            <xsl:variable name="LocalSrcLut">
              <xsl:choose>
                <xsl:when test="@LUT">
                  <xsl:value-of select="@LUT"/>
                </xsl:when>
               <xsl:otherwise>
                 <xsl:value-of select="$TrgSrcLut"/>
               </xsl:otherwise>
              </xsl:choose>
            </xsl:variable>

            <xsl:variable name="TrgSrcLine">
              <xsl:choose>
                <xsl:when test="@Line">
                  <xsl:value-of select="@Line"/>
                </xsl:when>
                <xsl:otherwise>
                  <xsl:value-of select="count(../../preceding-sibling::Line)"/>
                </xsl:otherwise>
              </xsl:choose>
            </xsl:variable>

            <xsl:variable name="TrgSrcElem">
              <xsl:choose>
                <xsl:when test="@Elem">
                  <xsl:value-of select="@Elem"/>
                </xsl:when>
                <xsl:otherwise>
                  <xsl:value-of select="count(../preceding-sibling::*)"/>
                </xsl:otherwise>
              </xsl:choose>
            </xsl:variable>

             <xsl:value-of select="concat('elem=el_',$LocalSrcLut,'_',$TrgSrcLine,'_',$TrgSrcElem)"/>
             <xsl:if test="not(position()=last())">
               <xsl:text>&amp;</xsl:text>
             </xsl:if>

          </xsl:for-each>
        </xsl:when>
        <xsl:otherwise>
          <xsl:value-of select="concat('elem=el_',$TrgSrcLut,'_',$RefSrcLine,'_',$RefSrcElem)"/>        
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>

    <xsl:variable name="Sources" select="concat('?',$RefSrcId,'%','?',$TrgSrcId,'%')"/>

    <xsl:variable name="SrcLuts" select="concat('#',$RefSrcLut,'%','#',$TrgSrcLut)"/>

    <xsl:call-template name="FillOneValue">
      <xsl:with-param name="SrcFile" select="$SrcFile"/>
      <xsl:with-param name="Sources" select="$Sources"/>
      <xsl:with-param name="SrcLut" select="$SrcLuts"/>

      <xsl:with-param name="Text">
        <xsl:call-template name="ConvertUnit">
          <xsl:with-param name="value"     select="@Value"/>
          <xsl:with-param name="unit_node" select="$unit_node"/>
        </xsl:call-template>
      </xsl:with-param>
      
    </xsl:call-template>
    
  </xsl:template>
  
  <xd:doc> Prints table internals for summary tables .
    <xd:param name="file" type="String">Part of the file name used as link.</xd:param>
    <xd:param name="unit_node" type="XMLNode">Unit node (see previous definitions)</xd:param>
    <xd:param name="SecId"    type="string">identifier for the section to be created.</xd:param>
  </xd:doc>
  <xsl:template name="FillOneValue_Line">
    
    <xsl:param name="file"/>
    <xsl:param name="unit_node"/>
    <xsl:param name="SecId"/>
    
    <!-- prepare elements of traceability -->
    <xsl:variable name="SrcFile">
      <xsl:choose>
        <xsl:when test="/ReducedData/@Name=$OSRatio or /ReducedData/@Name=$PDRatio">
          <xsl:value-of select="concat($file, '_', $Corner, $Extensions/Summary)"/>
        </xsl:when>
        <xsl:otherwise>
          <xsl:value-of select="concat('Cells/', ../../@id, '_', $Corner, $Extensions/Cell)"/>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>

    <xsl:variable name="Sources">
      <xsl:choose>
        <xsl:when test="./Src">
          <xsl:for-each select="./Src">
            <xsl:variable name="SrcLut">
              <xsl:value-of select="@LUT"/>
            </xsl:variable>

            <xsl:variable name="SrcLine">
              <xsl:choose>
                <xsl:when test="@Line">
                  <xsl:value-of select="@Line"/>
                </xsl:when>
                <xsl:otherwise>
                  <xsl:value-of select="count(../../preceding-sibling::Line)"/>
                </xsl:otherwise>
              </xsl:choose>
            </xsl:variable>

            <xsl:variable name="SrcElem">
              <xsl:choose>
                <xsl:when test="@Elem">
                  <xsl:value-of select="@Elem"/>
                </xsl:when>
                <xsl:otherwise>
                  <xsl:value-of select="count(../preceding-sibling::*)"/>
                </xsl:otherwise>
              </xsl:choose>
            </xsl:variable>

            <xsl:value-of select="concat('elem=el_',$SrcLut,'_',$SrcLine,'_',$SrcElem)"/>
            <xsl:if test="not(position()=last())">
              <xsl:text>&amp;</xsl:text>
            </xsl:if>

          </xsl:for-each>
        </xsl:when>
        <xsl:otherwise>

          <xsl:variable name="SrcLine">
            <xsl:value-of select="count(../preceding-sibling::Line)"/>
          </xsl:variable>
          <xsl:variable name="SrcElem">
            <xsl:value-of select="count(preceding-sibling::*)"/>
          </xsl:variable>
          <xsl:for-each select="../../Src/@LUT">
            <xsl:variable name="SrcLut">
              <xsl:value-of select="."/>
            </xsl:variable>
            <xsl:value-of select="concat('elem=el_',$SrcLut,'_',$SrcLine,'_',$SrcElem)"/>
            <xsl:if test="not(position()=last())">
              <xsl:text>&amp;</xsl:text>
            </xsl:if>            
          </xsl:for-each>

        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>

    <!-- lut used as link when tracing the elements back -->
    <xsl:variable name="FirstSrcLut">
      <xsl:choose>
        <xsl:when test="Src/@LUT">
          <xsl:value-of select="Src/@LUT"/>
        </xsl:when>
        <xsl:otherwise>
          <xsl:value-of select="../../Src[1]/@LUT"/>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>

    <xsl:call-template name="FillOneValue">
      <xsl:with-param name="Sources"  select="$Sources"/>
      <xsl:with-param name="SrcFile"  select="$SrcFile"/>
      <xsl:with-param name="SrcLut"   select="$FirstSrcLut"/>
      <xsl:with-param name="SecId"    select="$SecId"/>
      
      <xsl:with-param name="Text">
        <xsl:call-template name="ConvertUnit">
          <xsl:with-param name="value"     select="@Value"/>
          <xsl:with-param name="unit_node" select="$unit_node"/>
        </xsl:call-template>
      </xsl:with-param>
      
    </xsl:call-template>    
  </xsl:template>
  
  
  <xd:doc> This is the helper function that creates all the values inside the tables with their behaviors.
    <xd:param name="Text"     type="string">This value (cell content).</xd:param>
    <xd:param name="SecId"    type="string">identifier for the section to be created.</xd:param>
    <xd:param name="Sources"  type="string">ID of the element source(s)</xd:param>
    <xd:param name="SrcFile"  type="int">source file</xd:param>
    <xd:param name="SrcLut"   type="lut gid">gid of the source LUT</xd:param>
    <xd:param name="TgtLut"   type="int">This element LUT gid</xd:param>    
    <xd:param name="TgtLine"  type="int">This element line index</xd:param>
    <xd:param name="TgtElem"  type="int">This element index</xd:param>
    <xd:param name="TgtId"    type="string">ID of this element</xd:param>    
  </xd:doc>
  <xsl:template name="FillOneValue">
    <xsl:param name="Sources"/>
    <xsl:param name="SecId"/>
    <xsl:param name="SrcFile"/>
    <xsl:param name="SrcLut"/>
    <xsl:param name="Text"/>
    
    <xsl:param name="TgtLut" select="../../@gid"/>    
    <xsl:param name="TgtLine"  select="count(../preceding-sibling::Line)"/>
    <xsl:param name="TgtElem"  select="count(preceding-sibling::*)"/>
    <xsl:param name="TgtId" select="concat('el_',$TgtLut,'_',$TgtLine,'_',$TgtElem)"/>
    
    <td>
      <xsl:call-template name="CreateHighlightBehavior">
            <xsl:with-param name="TgtLut" select="$TgtLut"/>
      </xsl:call-template>
      
      <div class="num">
        
        <xsl:call-template name="CreateTooltipBehavior">
            <xsl:with-param name="TgtLut" select="$TgtLut"/>
        </xsl:call-template>
        
        <xsl:choose>
          
          <xsl:when test="../../../stats">
          
            <!-- Saving all values -->
            <xsl:for-each select="@*[name() != 'Plot']">
              <xsl:attribute name="{name()}">
                <xsl:variable name="attrName">
                  <xsl:value-of select="name()"/>
                </xsl:variable>
                <xsl:choose>
                  <xsl:when test="not(../../../../stats/*[name() = $attrName])">
                    <xsl:value-of select="."/>
                  </xsl:when>
                  <xsl:otherwise>
                    <xsl:value-of select="."/>
                  </xsl:otherwise>
                </xsl:choose>
              </xsl:attribute>
            </xsl:for-each>
            
            <!-- Filling table with default values -->
            <xsl:variable name="defNode" select="../../../stats/*[1]"/>
            <xsl:call-template name="exceeded">
              <xsl:with-param name="text">
                <xsl:value-of select="@*[name() = name($defNode)]"/>
              </xsl:with-param>
              <xsl:with-param name="exceeded" select="@*[name() = concat('t', name($defNode))]"/>
            </xsl:call-template>
            
          </xsl:when>
          
          <xsl:otherwise>
            <xsl:call-template name="exceeded">
              <xsl:with-param name="text"     select="$Text"/>
              <xsl:with-param name="exceeded" select="@tolExceeded"/>
            </xsl:call-template>
          </xsl:otherwise>
          
        </xsl:choose>
        
        <xsl:if test="@Plot">
          <xsl:call-template name="CreatePlotLink">
            <xsl:with-param name="title">
              <![CDATA[<p class="SubTitle">]]>
              <xsl:call-template name="Map">
                <xsl:with-param name="map" select="$MapNode/plot/timing"/>
              <xsl:with-param name="key" select="$SecId"/>
              </xsl:call-template>
              <![CDATA[</p>]]>
            </xsl:with-param>
            <xsl:with-param name="link">
              <xsl:value-of select="@Plot"/>
            </xsl:with-param>
            <xsl:with-param name="type">2waves</xsl:with-param>
            <xsl:with-param name="context">
              <![CDATA[<span class="weakComment"> ]]>
              <![CDATA[<br/>Cell <span class="comment">]]>
              <xsl:choose>
                <xsl:when test="../../../../../../../../@Name">
                  <xsl:value-of select="../../../../../../../../@Name"/>
                </xsl:when>
                <xsl:otherwise>
                  <xsl:value-of select="../../../../../../@Name"/>
                </xsl:otherwise>
              </xsl:choose>

              <![CDATA[</span><br/>Pin <span class="comment">]]>
              <xsl:call-template name="CreateRelatedPinStr">
                <xsl:with-param name="table" select="../../.."/>
              </xsl:call-template>

              <xsl:variable name="ContextStr">
                <xsl:call-template name="CreateContextStr">
                  <xsl:with-param name="table" select="../../.."/>
                </xsl:call-template>
              </xsl:variable>

              <xsl:if test="$ContextStr!=''">
                <![CDATA[<br/>]]>
                <xsl:value-of select="$ContextStr"/>
              </xsl:if>
              <![CDATA[</span></span>]]>
            </xsl:with-param>
          </xsl:call-template>
        </xsl:if>
        
        <!-- do not create tooltip for some tables (cell, ...)-->
        <xsl:if test="not(/Cell) and not($LibMode='normal' and /ReducedData[@Name='FP' or @Name='RP' or @Name='PWR'])">
          <xsl:call-template name="CreateToolTip">
            <xsl:with-param name="TgtLut"       select="$TgtLut"/>
            <xsl:with-param name="SrcFile"      select="$SrcFile"/>
            <xsl:with-param name="Sources"      select="$Sources"/>
            <xsl:with-param name="SrcLut"       select="$SrcLut"/>
          </xsl:call-template>
        </xsl:if>
        
      </div>      
    </td>    
  </xsl:template>
   
  
  <xd:doc> Helper to create highlight behavior.
    <xd:param name="TgtLut"   type="int">LUT index of the table.</xd:param>
    <xd:param name="TgtLine"  type="int">Line index of the LUT.</xd:param>
    <xd:param name="TgtElem"  type="int">Element index of the line.</xd:param>
  </xd:doc>
  <xsl:template name="CreateHighlightBehavior">
    <xsl:param name="TgtLut"   select="../../@gid"/>
    <xsl:param name="TgtLine"  select="count(../preceding-sibling::Line)"/>
    <xsl:param name="TgtElem"  select="count(preceding-sibling::*)"/>
    
    <!-- ignore if PDF -->
    <xsl:if test="$DocType!='PDF'">
      <!-- build id to use as reference, enabling highlight through query -->
      <xsl:variable name="id" select="concat('el_',$TgtLut,'_',$TgtLine,'_',$TgtElem)"/>
      
      <xsl:attribute name="id">
        <xsl:value-of select="$id"/>
      </xsl:attribute>
      <xsl:attribute name="onclick">highlight(this);</xsl:attribute>
      <xsl:attribute name="onmouseover">highlight(this);</xsl:attribute>
      <xsl:attribute name="onmouseout">highlight(this);</xsl:attribute>
      
    </xsl:if>
    
  </xsl:template>  
  
  <xd:doc> Helper to create tooltip behavior.
    <xd:param name="TgtLut"   type="int">LUT index of the table.</xd:param>
    <xd:param name="TgtLine"  type="int">Line index of the LUT.</xd:param>
    <xd:param name="TgtElem"  type="int">Element index of the line.</xd:param>
  </xd:doc>
  <xsl:template name="CreateTooltipBehavior">
    
    <xsl:param name="TgtLut"   select="../../@gid"/>
    <xsl:param name="TgtLine"  select="count(../preceding-sibling::Line)"/>
    <xsl:param name="TgtElem"  select="count(preceding-sibling::*)"/>

    <!-- ignore if PDF -->
    <xsl:if test="$DocType!='PDF'">
      <!-- build id allowing proper behavior -->
      <xsl:variable name="id" select="concat('el_',$TgtLut,'_',$TgtLine,'_',$TgtElem)"/>
      
      <xsl:attribute name="onmouseover">showTT('<xsl:value-of select="$id"/>_tt'); </xsl:attribute>
      <xsl:attribute name="onmouseout">hideTT(); </xsl:attribute>
      
    </xsl:if>
    
  </xsl:template>
    
  
  <xd:doc> Helper to create tooltip: traceability! 
    <xd:param name="Sources"  type="string">ID of the element source(s)</xd:param>
    <xd:param name="SrcFile"  type="int">source file</xd:param>
    <xd:param name="SrcLut"   type="lut gid">gid of the source LUT</xd:param>
    <xd:param name="TgtLut"   type="int">This element LUT gid</xd:param>    
    <xd:param name="TgtLine"  type="int">This element line index</xd:param>
    <xd:param name="TgtElem"  type="int">This element index</xd:param>
    <xd:param name="TgtId"    type="string">ID of this element</xd:param>
  </xd:doc>
  <xsl:template name="CreateToolTip">
    <xsl:param name="SrcFile"/>
    <xsl:param name="Sources"/>
    <xsl:param name="SrcLut"/>

    <xsl:param name="TgtLut"       select="../../@gid"/>
    <xsl:param name="TgtLine"      select="count(../preceding-sibling::Line)"/>
    <xsl:param name="TgtElem"      select="count(preceding-sibling::*)"/>
    
    <xsl:param name="TgtId" select="concat('el_',$TgtLut,'_',$TgtLine,'_',$TgtElem)"/>
    
    <xsl:if test="$DocType!='PDF'">     

      <!-- build traceability -->
      <div class="tooltip">
        <xsl:attribute name="id"><xsl:value-of select="$TgtId"/>_tt</xsl:attribute>
        <a>
          <xsl:attribute name="href">
            <xsl:choose>
              <!-- in case of a comparison table, use a javascript to open two windows -->
              <xsl:when test="substring($SrcFile,1,6)='%DIFF%'">
                javascript:openFrame('<xsl:value-of select="concat($SrcFile, $Sources, $SrcLut)"/>');
              </xsl:when>
              <xsl:when test="/*/@Name='CAP'">
                <xsl:value-of select="concat($SrcFile,'?elem=el_GateCapacitance_Pins/Pin/capacitance_',$SrcLut,'#GateCapacitance')"/>
              </xsl:when>
              <xsl:otherwise>
                <xsl:value-of select="concat($SrcFile,'?',$Sources,'#',$SrcLut)"/>
              </xsl:otherwise>
            </xsl:choose>
          </xsl:attribute>
          See the table from where<br/>this value was extracted
        </a>
      </div>      
    </xsl:if>
    
  </xsl:template>  
  
  
  <xd:doc> Helper to create table headers.
    <xd:param name="var_name_1" type="string">Name of the first template variable.</xd:param>
    <xd:param name="var_name_2" type="string">Name of the second template variable.</xd:param>
    <xd:param name="unit_node" type="XMLNode">Unit node (see previous description).</xd:param>
    <xd:param name="ignore_v2" type="char">y/n used to ignore the second variable.</xd:param>
    <xd:param name="header_colspan" type="integer">number of columns to be occupied by the variable names (two by default)</xd:param>
  </xd:doc>
  <xsl:template name="BuildTableHeader">
    
    <xsl:param name="var_name_1"/>
    <xsl:param name="var_name_2"/>
    <xsl:param name="unit_node"/>
    <xsl:param name="ignore_v2"/>
    <xsl:param name="header_colspan">2</xsl:param>
    <!-- count values to create colspan properly -->
    <xsl:variable name="VarCount">
      <xsl:choose>
        <xsl:when test="variable_2 and $ignore_v2='n'">
          <xsl:value-of select="count(variable_2/Values/Element)"/>
        </xsl:when>
        <xsl:otherwise>1</xsl:otherwise>
      </xsl:choose>
    </xsl:variable>
    <!-- colgroups are required in current FOP (PDF) version -->
    <colgroup>
      <col/><col/>
      <xsl:for-each select="variable_1/Values/Element">
        <xsl:choose>
          <xsl:when test="$ignore_v2='y'">
            <col/>
          </xsl:when>
          <xsl:when test="$ignore_v2='n'">
            <xsl:for-each select="../../../variable_2/Values/Element">
              <col/>
            </xsl:for-each>
          </xsl:when>
        </xsl:choose>
      </xsl:for-each>
    </colgroup>
    
    <thead>
      <!-- create header (first 2 lines of the tables), base on template values -->
      <tr>
        
        <xsl:element name="th">
          <xsl:attribute name="colspan">
            <xsl:value-of select="$header_colspan"/>
          </xsl:attribute>
          <xsl:value-of select="$var_name_1"/>
        </xsl:element>
        
        <xsl:for-each select="variable_1/Values/Element">
          <xsl:element name="th">
            
            <xsl:attribute name="colspan">
              <xsl:value-of select="$VarCount"/>
            </xsl:attribute>
            
            <!-- Create highlight to the entire column -->
            <xsl:if test="($DocType!='PDF' and $ignore_v2='y') or not(../../../variable_2)">
              <xsl:attribute name="onclick">colHighlight(this);</xsl:attribute>
            </xsl:if>
            
            <xsl:call-template name="ConvertUnit">
              <xsl:with-param name="pattern"   select="'0.00##'"/>
              <xsl:with-param name="value"     select="@Value"/>
              <xsl:with-param name="unit_node" select="$unit_node/var1"/>
              <xsl:with-param name="header"    select="'yes'"/>
            </xsl:call-template>
            
          </xsl:element>
        </xsl:for-each>
        
      </tr>
      <!-- create second variable if existent and not ignored -->
      <xsl:if test="variable_2 and $ignore_v2='n'">
        <tr>
          
          <xsl:element name="th">
            <xsl:attribute name="colspan">
              <xsl:value-of select="$header_colspan"/>
            </xsl:attribute>
            <xsl:value-of select="$var_name_2"/>
          </xsl:element>
          
          <xsl:for-each select="variable_1/Values/Element">
            <xsl:for-each select="../../../variable_2/Values/Element">
              <th>
                <!-- if not PDF create highlight entire column -->
                <xsl:if test="$DocType!='PDF'">
                  <xsl:attribute name="onclick">colHighlight(this);</xsl:attribute>
                </xsl:if>
                
                <xsl:call-template name="ConvertUnit">
                  <xsl:with-param name="pattern"   select="'0.00##'"/>
                  <xsl:with-param name="value"     select="@Value"/>
                  <xsl:with-param name="unit_node" select="$unit_node/var2"/>
                  <xsl:with-param name="header"    select="'yes'"/>
                </xsl:call-template>
                
              </th>
            </xsl:for-each>
          </xsl:for-each>
          
        </tr>
      </xsl:if>
      
    </thead>
    
  </xsl:template>
  
  
  
  <xd:doc> Helper to create summary table headers.
    <xd:param name="var_name_1" type="string">Name of the first template variable.</xd:param>
    <xd:param name="unit_node" type="XMLNode">Unit node (see previous description).</xd:param>
  </xd:doc>
  <xsl:template name="BuildTableHeader_LinApprox">
    
    <xsl:param name="var_name_1"/>
    <xsl:param name="unit_node"/>
    <!-- PDF generator requires the colgroup/col elements -->
    <colgroup>
      <col/><col/>
      <xsl:for-each select="variable_1/Values/Element">
        <col/>
      </xsl:for-each>
    </colgroup>
    
    <thead>
      <tr>
        
        <th colspan="2">
          <xsl:value-of select="$var_name_1"/>
        </th>
        
        <xsl:for-each select="variable_1/Values/Element">
          <th>
            <!-- create highlight entire col if not PDF -->
            <xsl:if test="$DocType!='PDF'">
              <xsl:attribute name="onclick">colHighlight(this);</xsl:attribute>
            </xsl:if>
            
            <xsl:call-template name="ConvertUnit">
              <xsl:with-param name="pattern"   select="'0.00##'"/>
              <xsl:with-param name="value"     select="@Value"/>
              <xsl:with-param name="unit_node" select="$unit_node/var1"/>
            </xsl:call-template>
            
          </th>
        </xsl:for-each>
        
      </tr>
    </thead>
    
  </xsl:template>
  
  
  <xd:doc> Creates the text of the last line of cell tables, the context line 
    <xd:param name="table" type="XMLNode">Table node.</xd:param>
  </xd:doc>
  <xsl:template name="CreateContextStr">
    
    <xsl:param name="table"/>
    
<!--<xsl:if test="$table/../@TimingSense!=''">  
      <xsl:value-of select="concat(' Timing Sense=&quot;',$table/../@TimingSense,'&quot;')"/>
    </xsl:if>-->
    
    <xsl:if test="$table/../@TimingSense!=''">
      <xsl:choose>
        <xsl:when test="$table/../@TimingSense='positive_unate'">* Positive Unate</xsl:when>
        <xsl:when test="$table/../@TimingSense='negative_unate'">* Negative Unate</xsl:when>
        <xsl:otherwise>Non-Unate</xsl:otherwise>
      </xsl:choose>
    </xsl:if>
    
    <xsl:if test="$table/../@When!=''">
      <xsl:value-of select="concat(' * When=&quot;',$table/../@When,'&quot;')"/>
    </xsl:if>
    
    <xsl:if test="$table/../@SdfCond!='' and $table/../@When=''">
      <xsl:value-of select="concat(' * Std Delay Format Condition=&quot;',$table/../@SdfCond,'&quot;')"/>
    </xsl:if>
    
    <xsl:if test="$table/../@TimingType!=''"> 
      <xsl:value-of select="concat(' * Timing Type=&quot;',$table/../@TimingType,'&quot;')"/>
    </xsl:if>
    
    <xsl:if test="$table/../@RelBusPins!=''"> 
      <xsl:value-of select="concat(' * Related Bus Pins=',$table/../@RelBusPins)"/>
    </xsl:if>
    
  </xsl:template>
  
  
  
  <xd:doc> Helper to create context in cell tables.
    <xd:param name="table" type="XMLNode">Table node.</xd:param>
    <xd:param name="colCount" type="int">Total amount of columns (required for colspan).</xd:param>
  </xd:doc>
  <xsl:template name="CreateContext">
    
    <xsl:param name="table"/>
    <xsl:param name="colCount">50</xsl:param>
    <!-- do not create context if no values are given -->
    <xsl:choose>
      <xsl:when test="$table/../@TimingSense!='' or $table/../@When!='' or $table/../@SdfCond!='' or $table/../@TimingType!='' or $table/../@RelBusPins!=''">
        <tr>
          <td>
            
            <xsl:attribute name="colspan">
              <xsl:value-of select="$colCount"/>
            </xsl:attribute> 
            
            <span class="comment">
              <xsl:call-template name="CreateContextStr">
                <xsl:with-param name="table" select="$table"/>
              </xsl:call-template>
            </span>
            
          </td>
        </tr>
      </xsl:when>
      <xsl:otherwise>
        <tr>
          <td>
            <xsl:attribute name="colspan">
              <xsl:value-of select="$colCount"/>
            </xsl:attribute> 
            <span class="comment"> * No Context</span>
          </td>
        </tr>
      </xsl:otherwise>
    </xsl:choose>
    
  </xsl:template>
  
</xsl:stylesheet>
