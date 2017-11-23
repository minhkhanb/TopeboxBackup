<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet version="1.0"
xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
xmlns:edmx="http://schemas.microsoft.com/ado/2007/06/edmx"
xmlns:d="http://schemas.microsoft.com/ado/2007/08/dataservices"
xmlns:schema_1_0="http://schemas.microsoft.com/ado/2006/04/edm"
xmlns:schema_1_1="http://schemas.microsoft.com/ado/2007/05/edm"
xmlns:schema_1_2="http://schemas.microsoft.com/ado/2008/09/edm"
xmlns:m="http://schemas.microsoft.com/ado/2007/08/dataservices/metadata">
<xsl:output method="text"/>
<!-- Service NameSpace-->
<xsl:variable name="service_namespace" select="concat(//schema_1_0:EntityType/../@Namespace, //schema_1_1:EntityType/../@Namespace, //schema_1_2:EntityType/../@Namespace)" />
<xsl:variable name="modified_service_namespace">
	<xsl:call-template name="cleanQuote">
	<xsl:with-param name="string">
	<xsl:value-of select="$service_namespace"/>
	</xsl:with-param>
</xsl:call-template>
</xsl:variable>
<!-- Default service URI passed externally -->
<xsl:param name="DefaultServiceURI"/>
<xsl:template match="/">
<xsl:apply-templates select="/edmx:Edmx/edmx:DataServices/schema_1_0:Schema | /edmx:Edmx/edmx:DataServices/schema_1_1:Schema | /edmx:Edmx/edmx:DataServices/schema_1_2:Schema"/>
</xsl:template>
<xsl:template match="/edmx:Edmx/edmx:DataServices/schema_1_0:Schema | /edmx:Edmx/edmx:DataServices/schema_1_1:Schema | /edmx:Edmx/edmx:DataServices/schema_1_2:Schema">
<xsl:apply-templates select="schema_1_0:EntityContainer | schema_1_1:EntityContainer | schema_1_2:EntityContainer"/>

<xsl:for-each select="schema_1_0:ComplexType | schema_1_1:ComplexType | schema_1_2:ComplexType">
<xsl:apply-templates select="."/>
</xsl:for-each>
<xsl:for-each select="schema_1_0:EntityType | schema_1_1:EntityType | schema_1_2:EntityType">
<xsl:apply-templates select="."/>
</xsl:for-each>
</xsl:template>


<!-- Generate container class -->
<xsl:template match="schema_1_0:EntityContainer | schema_1_1:EntityContainer | schema_1_2:EntityContainer">
<xsl:variable name="smallcase" select="'abcdefghijklmnopqrstuvwxyz'" />
<xsl:variable name="uppercase" select="'ABCDEFGHIJKLMNOPQRSTUVWXYZ'" />
 
- (NSString*) getDtoNameFromMetaData: (NSString*)metadata
{
<xsl:for-each select="schema_1_0:EntitySet | schema_1_1:EntitySet | schema_1_2:EntitySet">
  if ([metadata compare:@"<xsl:value-of select="@Name"/>"] == NSOrderedSame)
  {
<xsl:variable name = "Text" select="@EntityType"></xsl:variable>
<xsl:variable name = "modifyText" select="translate($Text, '.', '_')"></xsl:variable>
      return @"<xsl:value-of select="$modifyText"/>";
  }
</xsl:for-each>  
    return nil;
}

</xsl:template>

<xsl:template name="cleanQuote">
<xsl:param name="string" />
<xsl:if test="contains($string, '.')"><xsl:value-of
    select="substring-before($string, '.')" />_<xsl:call-template
    name="cleanQuote">
                <xsl:with-param name="string"><xsl:value-of
select="substring-after($string, '.')" />
                </xsl:with-param>
        </xsl:call-template>
</xsl:if>
<xsl:if test="not(contains($string, '.'))"><xsl:value-of
select="$string" />
</xsl:if>
</xsl:template>

</xsl:stylesheet>