<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
  <xsl:output method="text" encoding="UTF-8" />

<xsl:template match="/">
  <report>
    <xsl:apply-templates />
  </report>
</xsl:template>

<xsl:template match="xml/date">
  <xsl:text>[b]</xsl:text><xsl:value-of select="." /><xsl:text>[/b]</xsl:text>
</xsl:template>

<xsl:template match="xml/rounds/number">
  <xsl:text>[b][size=5]</xsl:text><xsl:value-of select="." /><xsl:text>[/size][/b]</xsl:text>
</xsl:template>

<xsl:template match="xml/attackers/title|xml/defenders/title">
  <xsl:text>[size=4]</xsl:text><xsl:value-of select="." /><xsl:text>[/size]</xsl:text>
</xsl:template>

<xsl:template match="xml/attackers/player/name">
  <xsl:text>[color=#00FF40][b][size=4]</xsl:text><xsl:value-of select="." /><xsl:text>[/size][/b]</xsl:text>
</xsl:template>
<xsl:template match="xml/defenders/player/name">
  <xsl:text>[color=#FD8002][b][size=4]</xsl:text><xsl:value-of select="." /><xsl:text>[/size][/b]</xsl:text>
</xsl:template>

<xsl:template match="xml/attackers/player/coords">
  <xsl:value-of select="." /><xsl:text>[/color]</xsl:text>
</xsl:template>
<xsl:template match="xml/defenders/player/coords">
  <xsl:value-of select="." /><xsl:text>[/color]</xsl:text>
</xsl:template>


<xsl:template match="xml/attackers/player/fleet/ship/number">
  <xsl:text>[color=#00FF40]</xsl:text><xsl:value-of select="." /><xsl:text>[/color]</xsl:text>
</xsl:template>
<xsl:template match="xml/defenders/player/fleet/ship/number">
  <xsl:text>[color=#FD8002]</xsl:text><xsl:value-of select="." /><xsl:text>[/color]</xsl:text>
</xsl:template>

<xsl:template match="xml/*/player/techs/tech/value">
  <xsl:text>[b]</xsl:text><xsl:apply-templates /><xsl:text>[/b]</xsl:text>
</xsl:template>

<xsl:template match="xml/attackers/player/fleet/ship/lost|xml/defenders/player/fleet/ship/lost">
  <xsl:text>[color=#00D0D0]</xsl:text><xsl:value-of select="." /><xsl:text>[/color]</xsl:text>
</xsl:template>

<xsl:template match="xml/attackers/player/losses/total/number">
  <xsl:text>[color=#00AA50][b]</xsl:text><xsl:value-of select="." /><xsl:text>[/b][/color]</xsl:text>
</xsl:template>
<xsl:template match="xml/defenders/player/losses/total/number">
  <xsl:text>[color=#ED7010][b]</xsl:text><xsl:value-of select="." /><xsl:text>[/b][/color]</xsl:text>
</xsl:template>

<xsl:template match="xml/*/player/losses/individual">
  <xsl:text>[i]</xsl:text><xsl:apply-templates /><xsl:text>[/i]</xsl:text>
</xsl:template>

<xsl:template match="xml/attackers/player/losses/individual/resource">
  <xsl:text>[color=#00AA50]</xsl:text><xsl:value-of select="." /><xsl:text>[/color]</xsl:text>
</xsl:template>
<xsl:template match="xml/defenders/player/losses/individual/resource">
  <xsl:text>[color=#ED7010]</xsl:text><xsl:value-of select="." /><xsl:text>[/color]</xsl:text>
</xsl:template>

<xsl:template match="xml/winner">
  <xsl:text>[b][size=3]</xsl:text><xsl:value-of select="." /><xsl:text>[/size][/b]</xsl:text>
</xsl:template>

<xsl:template match="xml/captures/title">
  <xsl:text>[b]</xsl:text><xsl:value-of select="." /><xsl:text>[/b]</xsl:text>
</xsl:template>

<xsl:template match="xml/captures/resource">
  <xsl:text>[color=#00FF40]</xsl:text><xsl:value-of select="." /><xsl:text>[/color]</xsl:text>
</xsl:template>

<xsl:template match="xml/losses/title">
  <xsl:text>[b]</xsl:text><xsl:value-of select="." /><xsl:text>[/b]</xsl:text>
</xsl:template>

<xsl:template match="xml/losses/losses[@role = 'attacker']/total/number">
  <xsl:text>[color=#00AA50][size=4]</xsl:text><xsl:value-of select="." /><xsl:text>[/size][/color]</xsl:text>
</xsl:template>

<!--<xsl:template match="xml/losses/losses/individual">
  <xsl:text>[i]</xsl:text><xsl:apply-templates /><xsl:text>[/i]</xsl:text>
</xsl:template>

<xsl:template match="xml/losses/losses[@role = 'attacker']/individual/resource">
  <xsl:text>[color=#00AA50]</xsl:text><xsl:value-of select="." /><xsl:text>[/color]</xsl:text>
</xsl:template>-->

<xsl:template match="xml/losses/losses[@role = 'defender']/total/number">
  <xsl:text>[color=#ED7010][size=4]</xsl:text><xsl:value-of select="." /><xsl:text>[/size][/color]</xsl:text>
</xsl:template>

<!--<xsl:template match="xml/losses/losses[@role = 'defender']/individual/resource">
  <xsl:text>[color=#ED7010]</xsl:text><xsl:value-of select="." /><xsl:text>[/color]</xsl:text>
</xsl:template>-->

<xsl:template match="xml/losses/losses[@role = 'total']/total/number">
  <xsl:text>[size=5][b]</xsl:text><xsl:value-of select="." /><xsl:text>[/b][/size]</xsl:text>
</xsl:template>

<!--<xsl:template match="xml/losses/losses[@role = 'total']/individual/resource">
  <xsl:text>[b]</xsl:text><xsl:value-of select="." /><xsl:text>[/b]</xsl:text>
</xsl:template>-->

<xsl:template match="xml/debris/title">
  <xsl:text>[b]</xsl:text><xsl:value-of select="." /><xsl:text>[/b]</xsl:text>
</xsl:template>

<xsl:template match="xml/debris/resource">
  <xsl:text>[color=#ED7010][b]</xsl:text><xsl:value-of select="." /><xsl:text>[/b][/color]</xsl:text>
</xsl:template>

<xsl:template match="xml/yield/title">
  <xsl:text>[b]</xsl:text><xsl:value-of select="." /><xsl:text>[/b]</xsl:text>
</xsl:template>

<xsl:template match="xml/yield/player/*/individual">
  <xsl:text>[i]</xsl:text><xsl:apply-templates /><xsl:text>[/i]</xsl:text>
</xsl:template>

<xsl:template match="xml/yield/player[@role = 'attacker']/recycling/percent">
  <xsl:text>[color=#00AA50][size=3][b]</xsl:text><xsl:value-of select="." /><xsl:text>[/b][/size]</xsl:text>
</xsl:template>

<xsl:template match="xml/yield/player/*/total/number">
  <xsl:text>[size=3]</xsl:text><xsl:value-of select="." /><xsl:text>[/size][/color]</xsl:text>
</xsl:template>

<xsl:template match="xml/yield/player[@role = 'attacker']/recycling/individual/resource">
  <xsl:text>[color=#00AA50]</xsl:text><xsl:value-of select="." /><xsl:text>[/color]</xsl:text>
</xsl:template>

<xsl:template match="xml/yield/player[@role = 'attacker']/wrecycling/percent">
  <xsl:text>[color=#E6E600][size=3][b]</xsl:text><xsl:value-of select="." /><xsl:text>[/b][/size]</xsl:text>
</xsl:template>

<!--<xsl:template match="xml/yield/player[@role = 'attacker']/wrecycling/total/number">
  <xsl:value-of select="." /><xsl:text>[/color]</xsl:text>
</xsl:template>-->

<xsl:template match="xml/yield/player[@role = 'attacker']/wrecycling/individual/resource">
  <xsl:text>[color=#E6E600]</xsl:text><xsl:value-of select="." /><xsl:text>[/color]</xsl:text>
</xsl:template>

<xsl:template match="xml/yield/player[@role = 'defender']/recycling/percent">
  <xsl:text>[color=#ED7010][size=3][b]</xsl:text><xsl:value-of select="." /><xsl:text>[/b][/size]</xsl:text>
</xsl:template>

<!--<xsl:template match="xml/yield/player[@role = 'defender']/recycling/total/number">
  <xsl:value-of select="." /><xsl:text>[/color]</xsl:text>
</xsl:template>-->

<xsl:template match="xml/yield/player[@role = 'defender']/recycling/individual/resource">
  <xsl:text>[color=#ED7010]</xsl:text><xsl:value-of select="." /><xsl:text>[/color]</xsl:text>
</xsl:template>

<xsl:template match="xml/signature/title">
  <xsl:text>[size=2][color=orange]</xsl:text><xsl:value-of select="." /><xsl:text>[/color][/size]</xsl:text>
</xsl:template>

<xsl:template match="xml/signature/url">
  <xsl:text>[size=3][b][url=</xsl:text><xsl:value-of select="." /><xsl:text>]OZipper[/url][/b][/size]</xsl:text>
</xsl:template>

<xsl:template match="xml/moon/chance">
  <xsl:text>[size=4]</xsl:text><xsl:value-of select="." /><xsl:text>[/size]</xsl:text>
</xsl:template>

<xsl:template match="xml/moon/formed">
  <xsl:text>[b]</xsl:text><xsl:value-of select="." /><xsl:text>[/b]</xsl:text>
</xsl:template>

</xsl:stylesheet>
