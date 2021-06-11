<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
	<xsl:template match="/">
		<html>
			<head>
				<title>Irrigation System Log</title>
				<style>
    html {
      font-family: "Open Sans", sans-serif;
    }

    table {
      border-collapse: collapse;
	  font-size:large;
    }

	caption {
      padding: 10px;
	  font-size: 200%;
    }

	th, td {
      text-align: left;
      border: 1px solid rgb(190,190,190);
	  color: rgb(70, 70, 70);
      padding: 7px 18px;
	}

	caption, th, tr:nth-child(even) {
      background-color: rgb(245, 245, 245);
    }

	.nowordwrap {
	  white-space: nowrap;
	}

				</style>
				<meta name="viewport" content="initial-scale=1"/>
			</head>
			<body>
				<table>
					<caption>Irrigation System Log</caption>
					<thead>
						<tr>
							<th>Date</th>
							<th>Level</th>
							<th>Thread</th>
							<th>Message</th>
						</tr>
					</thead>
					<tbody>
						<xsl:for-each select="log_entries/log_entry">
							<tr>
								<td>
									<div class="nowordwrap"><xsl:value-of select="time"/></div>
								</td>
								<td><xsl:value-of select="level"/></td>
								<td><xsl:value-of select="thread"/></td>
								<td><xsl:value-of select="text"/></td>
							</tr>
						</xsl:for-each>
					</tbody>
				</table>
			</body>
		</html>
	</xsl:template>
</xsl:stylesheet>