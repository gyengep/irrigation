<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
	<xsl:template match="/">
		<html>
			<head>
				<title>Irrigation System</title>
				<meta name="viewport" content="width=device-width, initial-scale=1"/>
				<link rel="stylesheet" href="/css/w3.css"/>
				<link rel="stylesheet" href="/css/my-styles.css"/>
				<script src="/scripts/my-scripts.js"></script>
				<script src="//ajax.googleapis.com/ajax/libs/jquery/1.9.1/jquery.min.js"></script>
			</head>
			<body class="my-container">
					
				<table class="my-table w3-hoverable">
					<xsl:for-each select="programs/program">
						<xsl:variable name="programId"><xsl:value-of select="@id"/></xsl:variable>
						<xsl:variable name="adjustment"><xsl:value-of select="adjustment"/></xsl:variable>
						<xsl:variable name="enabled"><xsl:value-of select="enabled"/></xsl:variable>
						
						<tr>
							<td class="w3-row">
								
								<div class="w3-col s11" onclick="window.location.href='/api/v1/programs/{$programId}?programId={$programId}'">
									<div class="my-cell-caption"><xsl:value-of select="name"/></div>
									<div class="my-cell-content"><xsl:value-of select="schedulertype"/> - <xsl:value-of select="format-number(adjustment*0.01, '#%')"/></div>
								</div>
									
								<div class="w3-col s1">
									<label class="switch">
										<input type="checkbox" onclick="sendEnableOrDisableProgram({$programId}, this.checked)">
											<xsl:if test="enabled='true'">
												<xsl:attribute name="checked">checked</xsl:attribute>
											</xsl:if>
										</input>
										<span class="slider round"/>
									</label>
								</div>
								
							</td>
						</tr>
						
					</xsl:for-each>
				</table>
				
			</body>
		</html>
	</xsl:template>
</xsl:stylesheet>
