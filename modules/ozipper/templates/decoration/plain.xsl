<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns="http://www.w3.org/TR/REC-html40">
  <xsl:output method="text" encoding="UTF-8" />

<xsl:template match="/">
  <report>
    <xsl:apply-templates />
  </report>
</xsl:template>

</xsl:stylesheet>
