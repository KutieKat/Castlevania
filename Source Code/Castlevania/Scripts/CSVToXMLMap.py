import sys

# Constants
MAP_FILE_ARG = 1
TILE_WIDTH_ARG = 2
TILE_HEIGHT_ARG = 3
TILESET_WIDTH_ARG = 4
TILESET_HEIGHT_ARG = 5

# Read value from arguments
mapFile = sys.argv[MAP_FILE_ARG]
tileWidth = sys.argv[TILE_WIDTH_ARG]
tileHeight = sys.argv[TILE_HEIGHT_ARG]
tilesetWidth = sys.argv[TILESET_WIDTH_ARG]
tilesetHeight = sys.argv[TILESET_HEIGHT_ARG]

# Parse the map file and create xml file content
xmlFileContent = "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
xmlFileContent += "<map tileWidth=\"" + tileWidth + "\" tileHeight=\"" + tileHeight + "\" tilesetWidth=\"" + tilesetWidth + "\" tilesetHeight=\"" + tilesetHeight + "\">\n\t"

with open(mapFile, "r") as filestream:
	for row in filestream:
		rowContent = "<row>\n\t"

		columns = row.split(" ")

		for column in range(0, len(columns)):
			columnContent = "<column>"
			value = str(columns[column]).strip()

			if "\n" not in value:
				columnContent += value

			columnContent += "</column>"
			rowContent += columnContent

		rowContent += "</row>"
		xmlFileContent += rowContent

	xmlFileContent += "</map>"

# Save map file content into an xml file
xmlFile = open("Map.xml","w+")
xmlFile.writelines(xmlFileContent)
xmlFile.close()