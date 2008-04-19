<?xml version="1.0" encoding="ISO-8859-1"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:xd="http://www.pnp-software.com/XSLTdoc" xmlns:fn="http://www.w3.org/2005/02/xpath-functions">
  
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
    <xd:cvsId>$Id: Utils.xsl,v 1.9 2008/01/15 18:52:59 rgs Exp $</xd:cvsId>
    <xd:short>Utility package</xd:short>
    <xd:detail>Contains several templates used in many places throughout the scripts</xd:detail>
  </xd:doc>
  
  
  
  <xd:doc>
    Creates the notes section for the comparison pages
    <xd:param name="CornerNode" type="element">a XML element containing the corner information</xd:param>
    <xd:param name="Page" type="string">name of the page for which the notes are being built</xd:param>
    <xd:param name="PDFLink" type="string">link to be used in the PDF version of the Databook</xd:param>
  </xd:doc>
  <xsl:template name="BasicNotes">
    
    <xsl:param name="CornerNode"/>
    <xsl:param name="Page"/>
    <xsl:param name="PDFLink"/>
    
    <xsl:if test="$CornerNode/@Ref">
      
      <xsl:variable name="RefNode" select="$XmlCornerList/CornerList/Corner[@Id=$CornerNode/@Ref]"/>
      <xsl:variable name="TrgNode" select="$XmlCornerList/CornerList/Corner[@Id=$CornerNode/Trg/@Id[position()=1]]"/>
      
      <p><span><a></a></span>This is 
        <xsl:choose>
          <xsl:when test="$LibMode=$DevModeAbs">an absolute</xsl:when>
          <xsl:when test="$LibMode=$DevModeBest">a best case</xsl:when>
          <xsl:when test="$LibMode=$DevModeWorst">a worst case</xsl:when>
          <xsl:otherwise>a relative</xsl:otherwise>
        </xsl:choose> comparison datasheet between 
        <span class="bold">
          <xsl:call-template name="CreateCornerStr">
            <xsl:with-param name="Corner"  select="$RefNode"/>
            <xsl:with-param name="Page"    select="$Page"/>
            <xsl:with-param name="PDFLink" select="$PDFLink"/>
          </xsl:call-template>
        </span> and
        <span class="bold">
          <xsl:call-template name="CreateCornerStr">
            <xsl:with-param name="Corner"  select="$TrgNode"/>
            <xsl:with-param name="Page"    select="$Page"/>
            <xsl:with-param name="PDFLink" select="$PDFLink"/>
          </xsl:call-template>
        </span>.
      </p>
      
      <xsl:if test="$LibMode=$DevMode">
        <p>The units of the tables represent the calculation of the comparison values using: (target/reference) - 1.</p>
      </xsl:if>
      
    </xsl:if>

  </xsl:template>
  
  
  
  <!-- Helper to create corner info in all pages -->
  <xd:doc>
    Builds the corner information that is shown in the notes section
    <xd:param name="Corner" type="element">a XML element containing corner information</xd:param>
    <xd:param name="Page" type="string">name of the page for which the notes are being built</xd:param>
    <xd:param name="PDFLink" type="string">link to be used in the PDF version of the Databook</xd:param>
    <xd:param name="SrcName" type="string">name of the root element in the page being processed. Automatically fetched by the script</xd:param>
  </xd:doc>
  <xsl:template name="Corner">
    
    <xsl:param name="Corner"/>
    <xsl:param name="Page"/> 
    <xsl:param name="PDFLink"/> 
    <xsl:param name="SrcName" select="name(/*)"/>

<!--<xsl:choose>
      <xsl:when test="$Corner/@Name=$NoCorner">
        <p class="error">Corner information not available.</p>
      </xsl:when>
      <xsl:otherwise>-->
        
        <xsl:for-each select="/*/StatDescr/Param">
          <p><b><xsl:value-of select="@Name"/>: </b><xsl:value-of select="@Descr"/></p>
        </xsl:for-each>

        <p>
          
          <xsl:if test="$Corner/@Voltage or $Corner/@Temperature"><span><a></a></span>Conditions for characterization library 
            <span class="bold"><xsl:value-of select="$Corner/@LibName"/></span>
            <xsl:if test="substring($Corner/@Name,1,9)!=$NoCorner">, corner 
              <span class="bold"><xsl:value-of select="$Corner/@Name"/></span>
            </xsl:if>: 
            <xsl:if test="$Corner/@Voltage">Vdd=
              <span class="bold">
                <xsl:call-template name="ConvertUnit">
                  <xsl:with-param name="value" select="$Corner/@Voltage"/>
                  <xsl:with-param name="unit_node" select="$UnitNode/voltage"/>
                </xsl:call-template>
              </span>V
              <xsl:if test="$Corner/@Temperature">,</xsl:if>
            </xsl:if> 
            <xsl:if test="$Corner/@Temperature">Tj=
              <span class="bold">
                <xsl:call-template name="ConvertUnit">
                  <xsl:with-param name="value" select="$Corner/@Temperature"/>
                  <xsl:with-param name="unit_node" select="$UnitNode/temperature"/>
                </xsl:call-template>
              </span>deg.C
            </xsl:if>.
          </xsl:if>
          
          <xsl:variable name="Name" select="$Corner/@Name"/>
          
          <xsl:if test="$Corner/../Corner[@Name!=$Name]"><span><a></a></span>Additional corners:
            <xsl:for-each select="$Corner/../Corner[@Name!=$Name]">
              
              <xsl:if test="position()>1">, </xsl:if>
              
              <xsl:call-template name="CreateCornerStr">
                <xsl:with-param name="Page" select="$Page"/>
                <xsl:with-param name="PDFLink" select="$PDFLink"/>
                <xsl:with-param name="SrcName" select="$SrcName"/>
              </xsl:call-template>
              
            </xsl:for-each>.
          </xsl:if>
          
        </p>

  <!--</xsl:otherwise>
    </xsl:choose>-->
    
  </xsl:template>
  
  
  
  <!-- Helper to create the corner string -->
  <xd:doc>
    Builds a string identifying the corresponding to the current page on another corner
    <xd:param name="Corner" type="element">a XML element containing corner information</xd:param>
    <xd:param name="Page" type="string">name of the page for which the notes are being built</xd:param>
    <xd:param name="PDFLink" type="string">link to be used in the PDF version of the Databook</xd:param>
    <xd:param name="SrcName" type="string">name of the root element in the page being processed. Automatically fetched by the script</xd:param>
  </xd:doc>
  <xsl:template name="CreateCornerStr">

    <xsl:param name="Corner"  select="."/>
    <xsl:param name="Page"    select="''"/>
    <xsl:param name="PDFLink" select="''"/>
    <xsl:param name="SrcName" select="name(/*)"/>
    
    <a>
      <xsl:variable name="XmlLib" select="document(concat('../../', $Corner/@File))"/>
      
      <xsl:choose>
        <xsl:when test="($SrcName='Cell' and $XmlLib/Cells/CellType/Cell[@Name=$Page]) or
                        ($SrcName!='Cell' and $XmlLib/TimingSummary/Entry[@Link=$Page])">
          <xsl:attribute name="href">
            <xsl:choose>
              <xsl:when test="$Config/@doctype='PDF'">
                <xsl:value-of select="concat('@', $PDFLink,' [',$Corner/@Name,']')"/>
              </xsl:when>
              <xsl:otherwise>
                <xsl:value-of select="concat($Page,'_',$Corner/@Name,'.xml#Notes')"/>
              </xsl:otherwise>
            </xsl:choose>
          </xsl:attribute>
        </xsl:when>
        <xsl:otherwise>
          <xsl:choose>
            <xsl:when test="($Page='FirstOrderOutputSlope' or $Page='FirstOrderPropagationDelay') and ($Corner/@Name='Absolute_Deviation' or $Corner/@Name='Relative_Deviation')">
              <xsl:attribute name="title">This page is not available for this corner.</xsl:attribute>
            </xsl:when>
            <xsl:when test="$Page='Capacitance' and $Corner/@Name!='Absolute_Deviation' and $Corner/@Name!='Relative_Deviation'">
              <xsl:attribute name="title">This page is not available for this corner.</xsl:attribute>
            </xsl:when>
            <xsl:when test="$SrcName='Profilings'">
              <xsl:choose>
                <xsl:when test="$XmlLib/Library/Profilings/Plot/Plot[@File=$Page]">
                  <xsl:attribute name="href">
                    <xsl:value-of select="concat($Page, '_', $Corner/@Name, $Extensions/Profiling)"/>
                  </xsl:attribute>
                </xsl:when>
                <xsl:otherwise>
                  <xsl:attribute name="title">This page is not available for this corner.</xsl:attribute>
                </xsl:otherwise>
              </xsl:choose>
            </xsl:when>
            <xsl:when test="$SrcName='Cell'">
              <xsl:choose>
                <xsl:when test="$XmlLib/Library/Cells/CellType/Cell[@Name=$Page]">
                  <xsl:attribute name="href">
                    <xsl:value-of select="concat($Page, '_', $Corner/@Name, $Extensions/Cell)"/>
                  </xsl:attribute>
                </xsl:when>
                <xsl:otherwise>
                  <xsl:attribute name="title">This page is not available for this corner.</xsl:attribute>
                </xsl:otherwise>
              </xsl:choose>
            </xsl:when>
            <xsl:otherwise>
              <xsl:attribute name="href">
                <xsl:value-of select="concat($Page, '_', $Corner/@Name, $Extensions/Summary)"/>
              </xsl:attribute>
            </xsl:otherwise>
          </xsl:choose>
        </xsl:otherwise>
      </xsl:choose>
      
      <xsl:value-of select="concat($Corner/@LibName, ' [', $Corner/@Name, ']')"/>
    </a>
    
  </xsl:template>
  
  
  
  <!-- Helper to create the collapse behavior everywhere -->
  <xd:doc>
    Adds collapse behaviour to the current section of the page
    <xd:param name="name" type="string">name of the current section</xd:param>
    <xd:param name="tag" type="string">address used to link to this section. Default value is a copy of <i>name</i></xd:param>
    <xd:param name="dir" type="string">path to the point where the <i>resources</i> folder is located. Default is the current directory</xd:param>
    <xd:param name="textStyle" type="string">defines the style to be applied to the name of the section. Default is <i>Caption</i></xd:param>
    <xd:param name="hide" type="boolean">defines if the branch should start out hidden. Default value is <i>false</i></xd:param>
  </xd:doc>
  <xsl:template name="CreateCollapse">
    
    <xsl:param name="name"/>
    <xsl:param name="tag" select="$name"/>
    <xsl:param name="dir">.</xsl:param>
    <xsl:param name="textStyle">Caption</xsl:param>
    <xsl:param name="hide">false</xsl:param>
    <xsl:param name="extra"/>
    
<!--<xsl:variable name="link">#<xsl:value-of select="$tag"/></xsl:variable>-->
    <xsl:variable name="link">javascript:;</xsl:variable>
    
    <xsl:choose>
      <xsl:when test="$DocType!='PDF'">
        <table class="Caption">
          <colgroup><col/><col/></colgroup>
          <tr>
                
            <td>
            <xsl:attribute name="onClick">showBranch('_<xsl:value-of select="$tag"/>');</xsl:attribute>
              <a class="Caption">
                <xsl:attribute name="href"><xsl:value-of select="$link"/></xsl:attribute>
                <span>
                  <xsl:attribute name="id">I_<xsl:value-of select="$tag"/></xsl:attribute>
                  <img border="0">
                    <xsl:attribute name="src"><xsl:value-of select="$dir"/>/resources/image/<xsl:choose><xsl:when test="$hide='true'">close</xsl:when><xsl:otherwise>open</xsl:otherwise></xsl:choose>_square.gif</xsl:attribute>
                  </img>
                </span>&#160;&#160;
              </a>
            </td>
            
            <td>
            <xsl:attribute name="onClick">showBranch('_<xsl:value-of select="$tag"/>');</xsl:attribute>
              <xsl:element name="a">
                <xsl:attribute name="class"><xsl:value-of select="$textStyle"/></xsl:attribute>
                <xsl:attribute name="href"><xsl:value-of select="$link"/></xsl:attribute>
                <xsl:value-of select="$name"/>
              </xsl:element>
            </td>
            <td>
               <xsl:text>&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;</xsl:text>
              <xsl:element name="span">
                <xsl:attribute name="class">extra</xsl:attribute>
                <xsl:copy-of select="$extra"/>
              </xsl:element>
            </td>
          </tr>
        </table>
      </xsl:when>
      <xsl:otherwise>
        <xsl:element name="p">
          <xsl:attribute name="class"><xsl:value-of select="$textStyle"/></xsl:attribute>
          <xsl:value-of select="$name"/>
        </xsl:element>
      </xsl:otherwise>
    </xsl:choose>
    
  </xsl:template>
  
  
  
  <xd:doc>
    Returns the mapped value associated to the given key
    <xd:param name="key" type="string">name of the key whose value is being queried</xd:param>
    <xd:param name="map" type="element">a XML element containing the pairs (key, value). Default value uses the local variable <i>MapNode</i></xd:param>
  </xd:doc>
  <xsl:template name="Map">
    
    <xsl:param name="key"/>
    <xsl:param name="map" select="$MapNode"/>
    
    <xsl:choose>
      <xsl:when test="$map/entry[@key=normalize-space($key)]">
        <xsl:value-of select="$map/entry[@key=normalize-space($key)]/@value"/>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="concat('UNKNOWN_KEY=',$key)"/>
      </xsl:otherwise>
    </xsl:choose>
    
  </xsl:template>
  
  
  
  <!-- Helper to print values with tolerance exceeded -->
  <xd:doc>
    Outputs the given text indicating if it exceeds the expected tolerance
    <xd:param name="text" type="string">text to be copied to the output</xd:param>
    <xd:param name="exceeded" type="string">if set to "up" or "down", applies the appropriate style to <i>text</i></xd:param>
  </xd:doc>
  <xsl:template name="exceeded">
    
    <xsl:param name="text"/>
    <xsl:param name="exceeded"/>
    
    <xsl:choose>
      <xsl:when test="$exceeded='up'">
        <span class="errorUp"><xsl:value-of select="$text"/></span>
      </xsl:when>
      <xsl:when test="$exceeded='down'">
        <span class="errorDown"><xsl:value-of select="$text"/></span>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="$text"/>
      </xsl:otherwise>
    </xsl:choose>
    
  </xsl:template>
  
  
  
  <xd:doc>
    Uses JavaScript to create a link that opens the plot image in a new window
    <xd:param name="link" type="string">points to the image file</xd:param>
    <xd:param name="alt" type="string">text that is presented if the plot icon cannot be found</xd:param>
    <xd:param name="title" type="string">title to be displayed in the plot window</xd:param>
    <xd:param name="context" type="string">contains HTML data with context information for the plot</xd:param>
    <xd:param name="type" type="string">type of plot ('lut', '2waves', '4waves', 'volt')</xd:param>
    <xd:param name="count" type="integer">indicantes the index of the last plot when type is '4waves'</xd:param>
  </xd:doc>
  <xsl:template name="CreatePlotLink">
    
    <xsl:param name="link"/>
    <xsl:param name="alt"/>
    <xsl:param name="title"/>	
    <xsl:param name="context"/>	
    <xsl:param name="type">lut</xsl:param>
    <xsl:param name="count"/>
    
    <xsl:if test="$DocType!='PDF'">
      <a>
        
        <xsl:variable name="width">
          <xsl:choose>
            <xsl:when test="$type='2waves'">700</xsl:when>
            <xsl:otherwise>600</xsl:otherwise>
          </xsl:choose>
        </xsl:variable>
        
        <xsl:variable name="height">550</xsl:variable>
        
        <xsl:attribute name="href">javascript:showPlot<xsl:if test="$type='4waves' or $type='volt'">List</xsl:if>
          ('<xsl:value-of select="$RootPath"/>',
          '<xsl:value-of select="concat('../../', $link)"/>',
          '<xsl:value-of select="$title"/>',
          '<xsl:value-of select="$context"/>',
          <xsl:value-of select="$width"/>,
          <xsl:value-of select="$height"/>
          <xsl:if test="$type='4waves' or $type='volt'">, <xsl:value-of select="$count"/></xsl:if>);
        </xsl:attribute>

        <xsl:choose>
          <xsl:when test="$alt">
            <xsl:value-of select="$alt"/>
          </xsl:when>
          <xsl:otherwise>
            
            <xsl:variable name="file">
              <xsl:choose>
                <xsl:when test="$type='4waves'">4waves_plot.gif</xsl:when>
                <xsl:when test="$type='volt'">volt_plot.gif</xsl:when>
                <xsl:otherwise>plot.gif</xsl:otherwise>
              </xsl:choose>
            </xsl:variable>
            
            <img border="0" onClick="disableHighlight()" >
              <xsl:attribute name="src">
                <xsl:value-of select="concat($RootPath, '/resources/image/', $file)"/>
              </xsl:attribute>
              <xsl:if test="$type='2waves'">
                <xsl:attribute name="style">display:none</xsl:attribute>
              </xsl:if>
            </img>
          </xsl:otherwise>
        </xsl:choose>

      </a>
    </xsl:if>
    
  </xsl:template>
  

  <xd:doc>
    Uses JavaScript to create a link that opens the CTP page in a new window
    <xd:param name="XmlLib" type="node">Root node containing CTP information</xd:param>
    <xd:param name="path" type="string">Path to databook folder. Default: current folder</xd:param>
    <xd:param name="type" type="string">Type of page, 'propagation' or 'slope'</xd:param>
    <xd:param name="unateness" type="string">Transition type</xd:param>
    <xd:param name="risefall" type="string">'rise' or 'fall' graph</xd:param>
  </xd:doc>

  <xsl:template name="CreateCTPLink">
    <xsl:param name="XmlLib"/>
    <xsl:param name="path">.</xsl:param>
    <xsl:param name="type"></xsl:param>
    <xsl:param name="unateness"></xsl:param>
    <xsl:param name="risefall"></xsl:param>


    <xsl:variable name="default_prop">50</xsl:variable>
    <xsl:variable name="default_slew_upper">80</xsl:variable>
    <xsl:variable name="default_slew_lower">20</xsl:variable>

    <xsl:variable name="input_threshold_rise">
      <xsl:choose>
        <xsl:when test="$XmlLib/input_threshold_pct_rise">
          <xsl:value-of select="number($XmlLib/input_threshold_pct_rise)"/>
        </xsl:when>
        <xsl:otherwise>
          <xsl:value-of select="$default_prop"/>
          <xsl:text>*</xsl:text>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>
    <xsl:variable name="input_threshold_fall">
      <xsl:choose>
        <xsl:when test="$XmlLib/input_threshold_pct_fall">
          <xsl:value-of select="number($XmlLib/input_threshold_pct_fall)"/>
        </xsl:when>
        <xsl:otherwise>
          <xsl:value-of select="$default_prop"/>
          <xsl:text>*</xsl:text>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>
    <xsl:variable name="output_threshold_rise">
      <xsl:choose>
        <xsl:when test="$XmlLib/output_threshold_pct_rise">
          <xsl:value-of select="number($XmlLib/output_threshold_pct_rise)"/>
        </xsl:when>
        <xsl:otherwise>
          <xsl:value-of select="$default_prop"/>
          <xsl:text>*</xsl:text>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>
    <xsl:variable name="output_threshold_fall">
      <xsl:choose>
        <xsl:when test="$XmlLib/output_threshold_pct_fall">
          <xsl:value-of select="number($XmlLib/output_threshold_pct_fall)"/>
        </xsl:when>
        <xsl:otherwise>
          <xsl:value-of select="$default_prop"/>
          <xsl:text>*</xsl:text>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>
    <xsl:variable name="slew_upper_fall">
      <xsl:choose>
        <xsl:when test="$XmlLib/slew_upper_threshold_pct_fall">
          <xsl:value-of select="number($XmlLib/slew_upper_threshold_pct_fall)"/>
        </xsl:when>
        <xsl:otherwise>
          <xsl:value-of select="$default_slew_upper"/>
          <xsl:text>*</xsl:text>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>
    <xsl:variable name="slew_lower_fall">
      <xsl:choose>
        <xsl:when test="$XmlLib/slew_lower_threshold_pct_fall">
          <xsl:value-of select="number($XmlLib/slew_lower_threshold_pct_fall)"/>
        </xsl:when>
        <xsl:otherwise>
          <xsl:value-of select="$default_slew_lower"/>
          <xsl:text>*</xsl:text>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>
    <xsl:variable name="slew_lower_rise">
      <xsl:choose>
        <xsl:when test="$XmlLib/slew_lower_threshold_pct_rise">
          <xsl:value-of select="number($XmlLib/slew_lower_threshold_pct_rise)"/>
        </xsl:when>
        <xsl:otherwise>
          <xsl:value-of select="$default_slew_lower"/>
          <xsl:text>*</xsl:text>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>
    <xsl:variable name="slew_upper_rise">
      <xsl:choose>
        <xsl:when test="$XmlLib/slew_upper_threshold_pct_rise">
          <xsl:value-of select="number($XmlLib/slew_upper_threshold_pct_rise)"/>
        </xsl:when>
        <xsl:otherwise>
          <xsl:value-of select="$default_slew_upper"/>
          <xsl:text>*</xsl:text>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>

    <xsl:variable name="pagetype">
      <xsl:choose>
        <xsl:when test="$type='propagation'">
          <xsl:choose>
            <xsl:when test="$unateness='positive'">
              <xsl:choose>
                <xsl:when test="$risefall='rise'">propposrise</xsl:when>
                <xsl:when test="$risefall='fall'">propposfall</xsl:when>
              </xsl:choose>
            </xsl:when>           
            <xsl:when test="$unateness='negative'">
              <xsl:choose>
                <xsl:when test="$risefall='rise'">propnegrise</xsl:when>
                <xsl:when test="$risefall='fall'">propnegfall</xsl:when>
              </xsl:choose>
            </xsl:when>
            <xsl:otherwise>
              <xsl:choose>
                <xsl:when test="$risefall='rise'">propbinrise</xsl:when>
                <xsl:when test="$risefall='fall'">propbinfall</xsl:when>
              </xsl:choose>
            </xsl:otherwise>
          </xsl:choose>
        </xsl:when>
        <xsl:when test="$type='slope'">
          <xsl:choose>
            <xsl:when test="$risefall='rise'">sloperise</xsl:when>
            <xsl:when test="$risefall='fall'">slopefall</xsl:when>
          </xsl:choose>
        </xsl:when>
      </xsl:choose>
    </xsl:variable>

    <xsl:variable name="value1">
      <xsl:choose>
        <xsl:when test="normalize-space($pagetype) = 'propposrise'">
          <xsl:value-of select="$input_threshold_rise"/>
        </xsl:when>
        <xsl:when test="normalize-space($pagetype) = 'propposfall'">
          <xsl:value-of select="$input_threshold_fall"/>
        </xsl:when>
        <xsl:when test="normalize-space($pagetype) = 'propnegrise'">
          <xsl:value-of select="$input_threshold_fall"/>
        </xsl:when>
        <xsl:when test="normalize-space($pagetype) = 'propnegfall'">
          <xsl:value-of select="$input_threshold_rise"/>
        </xsl:when>
        <xsl:when test="normalize-space($pagetype) = 'propbinrise'">
          <xsl:value-of select="$input_threshold_rise"/>
        </xsl:when>
        <xsl:when test="normalize-space($pagetype) = 'propbinfall'">
          <xsl:value-of select="$input_threshold_rise"/>
        </xsl:when>
        <xsl:when test="normalize-space($pagetype) = 'sloperise'">
          <xsl:value-of select="$slew_upper_rise"/>
        </xsl:when>
        <xsl:when test="normalize-space($pagetype) = 'slopefall'">
          <xsl:value-of select="$slew_upper_fall"/>
        </xsl:when>
      </xsl:choose>
    </xsl:variable>

    <xsl:variable name="value2">
      <xsl:choose>
        <xsl:when test="normalize-space($pagetype) = 'propposrise'">
          <xsl:value-of select="$output_threshold_rise"/>
        </xsl:when>
        <xsl:when test="normalize-space($pagetype) = 'propposfall'">
          <xsl:value-of select="$output_threshold_fall"/>
        </xsl:when>
        <xsl:when test="normalize-space($pagetype) = 'propnegrise'">
          <xsl:value-of select="$output_threshold_rise"/>
        </xsl:when>
        <xsl:when test="normalize-space($pagetype) = 'propnegfall'">
          <xsl:value-of select="$output_threshold_fall"/>
        </xsl:when>
        <xsl:when test="normalize-space($pagetype) = 'propbinrise'">
          <xsl:value-of select="$output_threshold_rise"/>
        </xsl:when>
        <xsl:when test="normalize-space($pagetype) = 'propbinfall'">
          <xsl:value-of select="$output_threshold_fall"/>
        </xsl:when>
        <xsl:when test="normalize-space($pagetype) = 'sloperise'">
          <xsl:value-of select="$slew_lower_rise"/>
        </xsl:when>
        <xsl:when test="normalize-space($pagetype) = 'slopefall'">
          <xsl:value-of select="$slew_lower_fall"/>
        </xsl:when>
      </xsl:choose>
    </xsl:variable>

    <xsl:variable name="value3">
      <xsl:choose>
        <xsl:when test="normalize-space($pagetype) = 'propbinrise'">
          <xsl:value-of select="$input_threshold_fall"/>
        </xsl:when>
        <xsl:when test="normalize-space($pagetype) = 'propbinfall'">
          <xsl:value-of select="$input_threshold_fall"/>
        </xsl:when>
      </xsl:choose>
    </xsl:variable>

    <xsl:variable name="graphnumber">
      <xsl:choose>
        <xsl:when test="starts-with($pagetype,'proppos')">2</xsl:when>
        <xsl:when test="starts-with($pagetype,'propneg')">2</xsl:when>
        <xsl:when test="starts-with($pagetype,'propbin')">3</xsl:when>
        <xsl:when test="starts-with($pagetype,'slope')">1</xsl:when>
      </xsl:choose>
    </xsl:variable>

    <a>    
      <xsl:attribute name="href">javascript:openctpPage(
      '<xsl:value-of select="$path"/>',
      '<xsl:value-of select="$graphnumber"/>',
      '<xsl:value-of select="$unateness"/>',
      '<xsl:value-of select="$risefall"/>',
      '<xsl:value-of select="$value1"/>',
      '<xsl:value-of select="$value2"/>',
      '<xsl:value-of select="$value3"/>');
      </xsl:attribute>
      <xsl:value-of select="$risefall"/>
    </a>  

  </xsl:template>
  
  <xd:doc>
    Builds the corner information that is displayed in the title notes
    <xd:param name="Page" type="string">name of the page for which the notes are being built</xd:param>
    <xd:param name="PDFLink" type="string">link to be used in the PDF version of the Databook</xd:param>
  </xd:doc>
  <xsl:template name="CreateTitleCornerNote">
    
    <xsl:param name="Page"/>
    <xsl:param name="PDFLink"/>
    
    <xsl:if test="substring(@CornerName,1,9)!=$NoCorner">
      
      <xsl:variable name="CornerName" select="@CornerName"/>
      <xsl:variable name="CornerNode" select="$XmlCornerList/CornerList/Corner[@Name=$CornerName]"/>
      
      <xsl:choose>
        <xsl:when test="$CornerNode/@Ref">
          
          <xsl:variable name="RefNode" select="$XmlCornerList/CornerList/Corner[@Id=$CornerNode/@Ref]"/>
          <xsl:variable name="TrgNode" select="$XmlCornerList/CornerList/Corner[@Id=$CornerNode/Trg/@Id[position()=1]]"/>
          
          Datasheet with
          <xsl:choose>
            <xsl:when test="$LibMode=$DevModeAbs">an absolute</xsl:when>
            <xsl:when test="$LibMode=$DevModeBest">a best case</xsl:when>
            <xsl:when test="$LibMode=$DevModeWorst">a worst case</xsl:when>
            <xsl:otherwise>a relative</xsl:otherwise>
          </xsl:choose>
          comparison data between 
          <span class="bold">
            <xsl:call-template name="CreateCornerStr">
              <xsl:with-param name="Corner"  select="$RefNode"/>
              <xsl:with-param name="Page"    select="$Page"/>
              <xsl:with-param name="PDFLink" select="$PDFLink"/>
            </xsl:call-template>
          </span> and
          <span class="bold">
            <xsl:call-template name="CreateCornerStr">
              <xsl:with-param name="Corner"  select="$TrgNode"/>
              <xsl:with-param name="Page"    select="$Page"/>
              <xsl:with-param name="PDFLink" select="$PDFLink"/>
            </xsl:call-template>
          </span>.
          The corner name is 
        </xsl:when>
        <xsl:otherwise>
          Datasheet for characterization corner: 
        </xsl:otherwise>
      </xsl:choose>
      
      <span class="bold"><xsl:value-of select="@CornerName"/></span>
      
      <xsl:if test="$XmlLibrary">
        , library <span class="bold"><xsl:value-of select="$XmlLibrary/Library/MainAttributes/Name"/></span>
      </xsl:if>.
      
    </xsl:if>
    
  </xsl:template>

  
  
  <xd:doc>
    Builds the profiling title, shown in the profiling pages and in the table of contents
  </xd:doc>
  <xsl:template name="buildProfilingTitle">
    <xsl:call-template name="Map">
      <xsl:with-param name="map" select="$MapNode/profiling_plots"/>
      <xsl:with-param name="key" select="@Name"/>
    </xsl:call-template>
    <xsl:choose>
      <xsl:when test="@Prefix">
        (<xsl:choose>
          <xsl:when test="@Prefix='Avg_Dev' or @Prefix='Worst_Dev'">Arc Based</xsl:when>
          <xsl:otherwise>
            <xsl:call-template name="Map">
              <xsl:with-param name="map" select="$MapNode/profiling_plots"/>
              <xsl:with-param name="key" select="@Prefix"/>
            </xsl:call-template>
          </xsl:otherwise>
        </xsl:choose>)
      </xsl:when>
      <xsl:when test="((count(../@Name)=0 and name(..)!='Profilings') or ../@Name!='cell') and @Name!='area' and @Name!='cell_leakage_power'"> (Cell Based)</xsl:when>
    </xsl:choose>
  </xsl:template>
  
</xsl:stylesheet>
