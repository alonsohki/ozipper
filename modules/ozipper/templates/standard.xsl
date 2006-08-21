<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
  <xsl:output method="xml" encoding="UTF-8" indent="yes" />

  <!-- Variables globales -->
  <xsl:variable name="nl">
<xsl:text>
</xsl:text>
  </xsl:variable>
  <xsl:variable name="tr" select="/xml/translation/string" />
  <xsl:variable name="month" select="/xml/translation/months/month" />
  <xsl:decimal-format name="decimal"
       decimal-separator=","
       grouping-separator="." />
  <xsl:variable name="mfmt">#.###</xsl:variable>
  <xsl:variable name="opts" select="/xml/options/option" />
  <xsl:variable name="yfmt">#%</xsl:variable>
  <xsl:decimal-format name="yield"
       decimal-separator=","
       grouping-separator="."
       infinity="Max"
       NaN="0%" />

<!-- Raíz -->
<xsl:template match="/">
  <xml>
    <xsl:apply-templates select="xml" />
  </xml>
</xsl:template>

<!-- Raíz del resultado -->
<xsl:template match="xml">
  <date>
    <title><xsl:apply-templates select="$tr[@id = 'battleofday']" /><xsl:text> </xsl:text></title>
    <day><xsl:apply-templates select="battle/date" /></day>
  </date>
  <text><xsl:value-of select="$nl" /></text>
  <rounds><xsl:apply-templates select="battle/rounds" /></rounds>
  <text><xsl:value-of select="$nl" />
  <xsl:value-of select="$nl" /></text>
  <xsl:apply-templates select="battle" />
</xsl:template>

<!-- Fecha -->
<xsl:template match="xml/battle/date">
  <xsl:value-of select="day" /><xsl:text> </xsl:text><xsl:apply-templates select="$tr[@id = 'of']" />
  <xsl:variable name="cmonth" select="month" />
  <xsl:text> </xsl:text><xsl:apply-templates select="$month[@id = $cmonth]" /><xsl:text>, </xsl:text>
  <xsl:apply-templates select="$tr[@id = 'at']" /><xsl:text> </xsl:text>
  <xsl:value-of select="format-number(hour, '00')" /><xsl:text>:</xsl:text>
  <xsl:value-of select="format-number(minute, '00')" /><xsl:text>:</xsl:text>
  <xsl:value-of select="format-number(second, '00')" />
</xsl:template>

<!-- Nº rondas -->
<xsl:template match="xml/battle/rounds">
  <xsl:apply-templates select="$tr[@id = 'brounds']" /><xsl:text> </xsl:text>
  <number><xsl:value-of select="." /></number><xsl:text> </xsl:text>
  <xsl:choose>
    <xsl:when test=". = 1">
      <xsl:apply-templates select="$tr[@id = 'round']" />
    </xsl:when>
    <xsl:otherwise>
      <xsl:apply-templates select="$tr[@id = 'rounds']" />
    </xsl:otherwise>
  </xsl:choose>
</xsl:template>

<!-- Traducción de meses -->
<xsl:template match="xml/translation/months/month">
  <xsl:value-of select="." />
</xsl:template>

<!-- Traducción de nombres de naves -->
<xsl:template match="xml/translation/ship">
  <xsl:value-of select="." />
</xsl:template>

<!-- Traducción general -->
<xsl:template match="xml/translation/string">
  <xsl:value-of select="." />
</xsl:template>

<!-- Datos de la batalla -->
<xsl:template match="xml/battle">
  <!-- Atacantes -->
  <attackers>
  <title>
    <name><xsl:apply-templates select="$tr[@id = 'attackers']" /></name>
    <number>
      <xsl:text> (</xsl:text>
       <xsl:value-of select="count(player[role = 'attacker'])" />
      <xsl:text>)</xsl:text>
    </number>
  </title>
  <xsl:apply-templates select="player[role = 'attacker']" />
  </attackers>
  <text><xsl:value-of select="$nl" /></text>
  
  <!-- Defensores -->
  <defenders>
  <title>
    <name><xsl:apply-templates select="$tr[@id = 'defenders']" /></name>
    <number>
      <xsl:text> (</xsl:text>
      <xsl:value-of select="count(player[role = 'defender'])" />
      <xsl:text>)</xsl:text>
    </number>
  </title>
  <xsl:apply-templates select="player[role = 'defender']" />
  </defenders>
  <text><xsl:value-of select="$nl" /></text>

  <!-- Resultado -->
  <xsl:apply-templates select="result" />
  <text><xsl:value-of select="$nl" /></text>

  <losses>
    <title><xsl:value-of select="$tr[@id = 'losses']" /></title>
    <text><xsl:value-of select="$nl" /></text>
  <!-- Pérdidas atacantes -->
  <losses role="attacker">
  <title><xsl:apply-templates select="$tr[@id = 'attackerfleetlosses']" /><xsl:text>: </xsl:text></title>
  <xsl:call-template name="total_losses">
    <xsl:with-param name="ploss" select="player[role = 'attacker']/losses" />
  </xsl:call-template>
  </losses>
  <text><xsl:value-of select="$nl" /></text>

  <!-- Pérdidas defensoras -->
  <losses role="defender">
  <title><xsl:apply-templates select="$tr[@id = 'defenderfleetlosses']" /><xsl:text>: </xsl:text></title>
  <xsl:call-template name="total_losses">
    <xsl:with-param name="ploss" select="player[role = 'defender']/losses" />
  </xsl:call-template>
  </losses>
  <text><xsl:value-of select="$nl" /></text>

  <!-- Pérdidas totales -->
  <losses role="total">
  <title><xsl:apply-templates select="$tr[@id = 'totallosses']" /><xsl:text>: </xsl:text></title>
  <xsl:call-template name="total_losses">
    <xsl:with-param name="ploss" select="player/losses" />
  </xsl:call-template>
  </losses>
  </losses>
  <text><xsl:value-of select="$nl" /><xsl:value-of select="$nl" /></text>

  <!-- Escombros -->
  <debris>
  <title><xsl:apply-templates select="$tr[@id = 'debris']" /></title>
  <xsl:variable name="total" select="debris/metal + debris/crystal" />
  <nrecyclers>
  <xsl:text> (</xsl:text>
  <xsl:value-of select="format-number(floor((($total - 1) div 20000) + 1), $mfmt, 'decimal')" /><xsl:text> </xsl:text>
  <xsl:value-of select="$tr[@id = 'recyclers']" />
  <xsl:text>)</xsl:text>
  </nrecyclers>
  <text><xsl:value-of select="$nl" /></text>
  <xsl:apply-templates select="debris" />
  </debris>
  <text><xsl:value-of select="$nl" /></text>

  <!-- Rentabilidad -->
  <yield>
  <title><xsl:apply-templates select="$tr[@id = 'yield']" /></title>
  <text><xsl:value-of select="$nl" /></text>
  <xsl:call-template name="yield">
    <xsl:with-param name="role">attacker</xsl:with-param>
  </xsl:call-template>
  <xsl:value-of select="$nl" />
  <xsl:call-template name="yield">
    <xsl:with-param name="role">defender</xsl:with-param>
  </xsl:call-template>
  </yield>
  <text><xsl:value-of select="$nl" /><xsl:value-of select="$nl" /></text>

  <!-- Luna -->
  <xsl:apply-templates select="moon" />

  <!-- Publi -->
  <signature><title><xsl:apply-templates select="$tr[@id = 'spam']" /><xsl:text> </xsl:text></title>
  <url><xsl:text>http://www.ozipper.net</xsl:text></url></signature>
</xsl:template>

<!-- Template para mostrar las pérdidas totales -->
<xsl:template name="total_losses">
  <xsl:param name="ploss" />
  <total>
  <number><xsl:value-of select="format-number(sum($ploss/metal|$ploss/crystal|$ploss/deuterium), $mfmt, 'decimal')" /><xsl:text> </xsl:text></number>
  <text><xsl:apply-templates select="$tr[@id = 'units']" /></text>
  </total>
  <!-- Unidades individuales -->
<!--  <xsl:if test="$opts[@id = 'individual-units'] = 'true'">
    <individual>
    <text><xsl:value-of select="$nl" /><xsl:text>(</xsl:text></text>
    <resource id="metal"><xsl:value-of select="format-number(sum($ploss/metal), $mfmt, 'decimal')" /><xsl:text> </xsl:text></resource>
    <units id="metal"><xsl:apply-templates select="$tr[@id = 'metal']" /><xsl:text>, </xsl:text></units>
    <resource id="crystal"><xsl:value-of select="format-number(sum($ploss/crystal), $mfmt, 'decimal')" /><xsl:text> </xsl:text></resource>
    <units id="crystal"><xsl:apply-templates select="$tr[@id = 'crystal']" /><xsl:text>, </xsl:text></units>
    <resource id="deuterium"><xsl:value-of select="format-number(sum($ploss/deuterium), $mfmt, 'decimal')" /><xsl:text> </xsl:text></resource>
    <units id="deuterium"><xsl:apply-templates select="$tr[@id = 'deuterium']" /></units>
    <text><xsl:text>)</xsl:text><xsl:value-of select="$nl" /></text>
    </individual>
  </xsl:if>-->
</xsl:template>

<!-- Datos individuales de cada jugador -->
<xsl:template match="xml/battle/player">
  <text><xsl:value-of select="$nl" /></text>
  <player>
    <name><xsl:value-of select="name" /></name>
    <coords><xsl:text> [</xsl:text><xsl:value-of select="coords" /><xsl:text>]</xsl:text></coords>
    <text><xsl:value-of select="$nl" /></text>
    <techs><xsl:apply-templates select="techs" /></techs>
    <text><xsl:value-of select="$nl" /></text>
    <fleet><xsl:apply-templates select="fleet" /></fleet>
    <xsl:apply-templates select="losses" />
  </player>
</xsl:template>

<!-- Tecnologías de un jugador -->
<xsl:template match="xml/battle/player/techs">
  <tech id="weapons">
    <name><xsl:apply-templates select="$tr[@id = 'weapons']" /><xsl:text>: </xsl:text></name>
    <value><xsl:value-of select="weapons" /><xsl:text>% </xsl:text></value>
  </tech>
  <tech id="shields">
    <name><xsl:apply-templates select="$tr[@id = 'shields']" /><xsl:text>: </xsl:text></name>
    <value><xsl:value-of select="shields" /><xsl:text>% </xsl:text></value>
  </tech>
  <tech id="armour">
    <name><xsl:apply-templates select="$tr[@id = 'armour']" /><xsl:text>: </xsl:text></name>
    <value><xsl:value-of select="armour" /><xsl:text>%</xsl:text></value>
  </tech>
</xsl:template>

<!-- Flotas de un jugador -->
<xsl:template match="xml/battle/player/fleet">
  <xsl:choose>
    <xsl:when test="count(ship) = 0">
      <xsl:apply-templates select="$tr[@id = 'noships']" />
      <text><xsl:value-of select="$nl" /></text>
    </xsl:when>
    <xsl:otherwise>
      <xsl:apply-templates select="ship" />
    </xsl:otherwise>
  </xsl:choose>
</xsl:template>

<xsl:template match="xml/battle/player/fleet/ship">
  <xsl:variable name="shipname" select="name" />
  <ship>
  <name><xsl:apply-templates select="/xml/translation/ship[@id = $shipname]" /><xsl:text> </xsl:text></name>
  <number><xsl:value-of select="format-number(initial, $mfmt, 'decimal')" /><xsl:text> </xsl:text></number>
  <lost>
  <title><xsl:apply-templates select="$tr[@id = 'lost']" /><xsl:text> </xsl:text></title>
  <number><xsl:value-of select="format-number(lost, $mfmt, 'decimal')" /></number>
  </lost>
  </ship>
  <text><xsl:value-of select="$nl" /></text>
</xsl:template>

<!-- Pérdidas individuales -->
<xsl:template match="xml/battle/player/losses">
  <losses>
  <title><xsl:apply-templates select="$tr[@id = 'losses']" /><xsl:text>: </xsl:text></title>
  <total>
  <number><xsl:value-of select="format-number(sum(metal|crystal|deuterium), $mfmt, 'decimal')" /><xsl:text> </xsl:text></number>
  <text><xsl:apply-templates select="$tr[@id = 'units']" /><xsl:text> </xsl:text></text>
  </total>
  <!-- Unidades individuales -->
  <xsl:if test="$opts[@id = 'individual-units'] = 'true'">
    <individual>
    <text><xsl:value-of select="$nl" /><xsl:text>(</xsl:text></text>
    <resource id="metal"><xsl:value-of select="format-number(metal, $mfmt, 'decimal')" /><xsl:text> </xsl:text></resource>
    <units id="metal"><xsl:apply-templates select="$tr[@id = 'metal']" /></units><text><xsl:text>, </xsl:text></text>
    <resource id="crystal"><xsl:value-of select="format-number(crystal, $mfmt, 'decimal')" /><xsl:text> </xsl:text></resource>
    <units id="crystal"><xsl:apply-templates select="$tr[@id = 'crystal']" /></units><text><xsl:text>, </xsl:text></text>
    <resource id="deuterium"><xsl:value-of select="format-number(deuterium, $mfmt, 'decimal')" /><xsl:text> </xsl:text></resource>
    <units id="deuterium"><xsl:apply-templates select="$tr[@id = 'deuterium']" /></units><text><xsl:text>)</xsl:text></text>
    </individual>
  </xsl:if>
  </losses>
  <text><xsl:value-of select="$nl" /></text>
</xsl:template>

<!-- Vencedor y botín -->
<xsl:template match="xml/battle/result">
  <xsl:variable name="winner" select="winner" />
  <winner><xsl:apply-templates select="$tr[@id = concat('winner_', $winner)]" /></winner><text><xsl:value-of select="$nl" /></text>
  <xsl:if test="$winner = 'attacker'">
    <xsl:apply-templates select="captures" />
    <text><xsl:value-of select="$nl" /></text>
  </xsl:if>
</xsl:template>

<xsl:template match="xml/battle/result/captures">
  <captures>
  <title><xsl:apply-templates select="$tr[@id = 'captures']" /><xsl:text>: </xsl:text></title>
  <resource id="metal"><xsl:value-of select="format-number(metal, $mfmt, 'decimal')" /><xsl:text> </xsl:text></resource>
  <units id="metal"><xsl:apply-templates select="$tr[@id = 'metal']" /></units><text><xsl:text>, </xsl:text></text>
  <resource id="crystal"><xsl:value-of select="format-number(crystal, $mfmt, 'decimal')" /><xsl:text> </xsl:text></resource>
  <units id="crystal"><xsl:apply-templates select="$tr[@id = 'crystal']" /><xsl:text> </xsl:text></units>
  <text><xsl:apply-templates select="$tr[@id = 'and']" /><xsl:text> </xsl:text></text>
  <resource id="deuterium"><xsl:value-of select="format-number(deuterium, $mfmt, 'decimal')" /><xsl:text> </xsl:text></resource>
  <units id="deuterium"><xsl:apply-templates select="$tr[@id = 'deuterium']" /></units>
  </captures>
</xsl:template>

<!-- Escombros -->
<xsl:template match="xml/battle/debris">
  <units id="metal"><xsl:apply-templates select="$tr[@id = 'metal']" /><xsl:text>: </xsl:text></units>
  <resource id="metal"><xsl:value-of select="format-number(metal, $mfmt, 'decimal')" /></resource>
  <text><xsl:text> </xsl:text><xsl:apply-templates select="$tr[@id = 'units']" /></text>
  <text><xsl:value-of select="$nl" /></text>
  <units id="crystal"><xsl:apply-templates select="$tr[@id = 'crystal']" /><xsl:text>: </xsl:text></units>
  <resource id="crystal"><xsl:value-of select="format-number(crystal, $mfmt, 'decimal')" /></resource>
  <text><xsl:text> </xsl:text><xsl:apply-templates select="$tr[@id = 'units']" /></text>
  <text><xsl:value-of select="$nl" /></text>
</xsl:template>

<!-- Rentabilidad -->
<xsl:template name="yield">
  <xsl:param name="role" />
  <player role="{$role}">
  <xsl:choose>
    <xsl:when test="$role = 'attacker'">
      <!-- Atacante reciclando -->
      <recycling>
      <title>
      <xsl:apply-templates select="$tr[@id = 'attackers']" /><xsl:text> (</xsl:text>
      <xsl:apply-templates select="$tr[@id = 'recycling']" /><xsl:text>): </xsl:text>
      </title>
      <xsl:call-template name="show-yield">
        <xsl:with-param name="losses" select="/xml/battle/player[role = $role]/losses" />
	<xsl:with-param name="capture" select="/xml/battle/result/captures" />
	<xsl:with-param name="debris" select="/xml/battle/debris" />
      </xsl:call-template>
      </recycling>
      <!-- Atacante sin reciclar -->
      <text><xsl:value-of select="$nl" /></text>
      <wrecycling>
      <title>
      <xsl:apply-templates select="$tr[@id = 'attackers']" /><xsl:text> (</xsl:text>
      <xsl:apply-templates select="$tr[@id = 'wrecycling']" /><xsl:text>): </xsl:text>
      </title>
      <xsl:call-template name="show-yield">
        <xsl:with-param name="losses" select="/xml/battle/player[role = $role]/losses" />
	<xsl:with-param name="capture" select="/xml/battle/result/captures" />
	<xsl:with-param name="debris" select="/xml/extra/nullresources" />
      </xsl:call-template>
      </wrecycling>
    </xsl:when>
    <xsl:otherwise>
      <!-- Defensor reciclando -->
      <recycling>
      <title>
      <xsl:apply-templates select="$tr[@id = 'defenders']" /><xsl:text> (</xsl:text>
      <xsl:apply-templates select="$tr[@id = 'recycling']" /><xsl:text>): </xsl:text>
      </title>
      <xsl:call-template name="show-yield">
        <xsl:with-param name="losses" select="/xml/battle/player[role = $role]/losses" />
	<xsl:with-param name="capture" select="/xml/extra/nullresources" />
	<xsl:with-param name="debris" select="/xml/battle/debris" />
      </xsl:call-template>
      </recycling>
    </xsl:otherwise>
  </xsl:choose>
  </player>
</xsl:template>

<xsl:template name="show-yield">
  <xsl:param name="losses" />
  <xsl:param name="capture" />
  <xsl:param name="debris" />
  <xsl:variable name="l" select="sum($losses/metal|$losses/crystal|$losses/deuterium)" />
  <xsl:variable name="b" select="sum($capture/metal|$capture/crystal|$capture/deuterium) + sum($debris/metal|$debris/crystal)" />
  <percent><xsl:text>[</xsl:text><xsl:value-of select="format-number(($b - $l) div $l, $yfmt, 'yield')" /><xsl:text>] </xsl:text></percent>
  <total>
  <number><xsl:value-of select="format-number(-$l + $b, $mfmt, 'decimal')" /><xsl:text> </xsl:text></number>
  <text><xsl:apply-templates select="$tr[@id = 'units']" /></text>
  </total>

  <xsl:if test="$opts[@id = 'individual-units'] = 'true'">
    <individual>
    <text><xsl:value-of select="$nl" /><xsl:text>(</xsl:text></text>
    <resource id="metal">
    <xsl:value-of select="format-number(-sum($losses/metal) + $capture/metal + $debris/metal, $mfmt, 'decimal')" />
    <xsl:text> </xsl:text>
    </resource>
    <units id="metal"><xsl:apply-templates select="$tr[@id = 'metal']" /></units><text><xsl:text>, </xsl:text></text>
    <resource id="crystal">
    <xsl:value-of select="format-number(-sum($losses/crystal) + $capture/crystal + $debris/crystal, $mfmt, 'decimal')" />
    <xsl:text> </xsl:text>
    </resource>
    <units id="crystal"><xsl:apply-templates select="$tr[@id = 'crystal']" /></units><text><xsl:text>, </xsl:text></text>
    <resource id="deuterium">
    <xsl:value-of select="format-number(-sum($losses/deuterium) + $capture/deuterium, $mfmt, 'decimal')" />
    <xsl:text> </xsl:text>
    </resource>
    <units id="deuterium"><xsl:apply-templates select="$tr[@id = 'deuterium']" /></units>
    <text><xsl:text>)</xsl:text><xsl:value-of select="$nl" /></text>
    </individual>
  </xsl:if>
</xsl:template>

<!-- Luna -->
<xsl:template match="xml/battle/moon">
  <xsl:if test="chance &gt; 0">
    <moon>
    <title><xsl:apply-templates select="$tr[@id = 'moonchance']" /><xsl:text> </xsl:text></title>
    <chance><xsl:value-of select="chance" /><xsl:text>%</xsl:text></chance>
    <text><xsl:value-of select="$nl" /></text>
    <xsl:if test="formed = 'true'">
      <formed><xsl:apply-templates select="$tr[@id = 'moonhit']" /></formed>
      <text><xsl:value-of select="$nl" /></text>
    </xsl:if>
    <text><xsl:value-of select="$nl" /></text>
    </moon>
  </xsl:if>
</xsl:template>

</xsl:stylesheet>
