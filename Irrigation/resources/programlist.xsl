<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
	<xsl:template match="/">
		<html>
			<head>
				<title>Irrigation System Programs</title>
				<meta name="viewport" content="width=device-width, initial-scale=1"/>
				<link rel="stylesheet" href="/resources/w3.css"/>
				<link rel="stylesheet" href="/resources/styles.css"/>
				<script src="//ajax.googleapis.com/ajax/libs/jquery/1.9.1/jquery.min.js"></script>
				<script src="/resources/scripts.js"></script>
			</head>
			<body>
				<div class="w3-container w3-sans-serif">

					<div class="w3-content w3-card w3-border" style="max-width:600px">
					
						<table class="w3-table w3-bordered w3-hoverable">
							<xsl:for-each select="programs/program">
								<xsl:variable name="programId"><xsl:value-of select="@id"/></xsl:variable>
								<xsl:variable name="adjustment"><xsl:value-of select="adjustment"/></xsl:variable>
							
								<tr onclick="window.location='/programs/{$programId}'">
																												
									<td class="w3-padding-16">

										<div class="w3-cell-row">
											<div class="w3-cell" style="width:90%">
												<b><xsl:value-of select="name"/></b>
												
												<div class="w3-row w3-text-grey">
												
													<div class="w3-col s1">
														<xsl:choose>
															<xsl:when test="enabled='true'">
																<input type="checkbox" onclick="return false" checked="checked"/>
															</xsl:when>
															<xsl:otherwise>
																<input type="checkbox" onclick="return false"/>
															</xsl:otherwise>
														</xsl:choose>
													</div>
													
													<div class="w3-col s2">
														<xsl:value-of select="format-number(adjustment*0.01, '#%')"/>
													</div>
													
													<div class="w3-rest"><xsl:value-of select="schedulertype"/></div>
												
												</div>
											</div>
										
											<div class="w3-cell w3-cell-middle">
												<div class="w3-right"><button class="w3-round" onclick="sendStart({$programId}, {$adjustment})">Start</button></div>
											</div>
										</div>
									</td>
									
										
								</tr>
							</xsl:for-each>
						</table>
					</div>
				</div>
			</body>
		</html>
	</xsl:template>
</xsl:stylesheet>
