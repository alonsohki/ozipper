<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
  <xsl:output method="text" encoding="UTF-8" />

<xsl:template match="/">
  <xml>
  <xsl:apply-templates />
  </xml>
</xsl:template>

<xsl:template match="xml/date">
  <xsl:text>[b]</xsl:text><xsl:value-of select="." /><xsl:text>[/b]</xsl:text>
</xsl:template>

<xsl:template match="xml/signature/title">
  <xsl:text>[size=12][color=red]</xsl:text><xsl:value-of select="." /><xsl:text>[/color]</xsl:text>
</xsl:template>

<xsl:template match="xml/signature/url">
  <xsl:text>[url=</xsl:text><xsl:value-of select="." /><xsl:text>]OZipper[/url][/size]</xsl:text>
</xsl:template>
  
</xsl:stylesheet>
