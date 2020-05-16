import sys
from lxml import etree
from xml.dom import minidom

# Constants
MAP_FILE_ARG = 1
TILE_WIDTH_ARG = 2
TILE_HEIGHT_ARG = 3
TILESET_WIDTH_ARG = 4
TILESET_HEIGHT_ARG = 5
OUTPUT_FILENAME = 'Map.xml'
ENCODING = 'utf-8'
BLANK_SPACE = ' '
DEFAULT_INDENT = BLANK_SPACE * 2
SUPPOSED_INDENT = BLANK_SPACE * 3

def parse():
    root = etree.Element("map")

    root.set('tileWidth', sys.argv[TILE_WIDTH_ARG])
    root.set('tileHeight', sys.argv[TILE_HEIGHT_ARG])
    root.set('tilesetWidth', sys.argv[TILESET_WIDTH_ARG])
    root.set('tilesetHeight', sys.argv[TILESET_HEIGHT_ARG])

    with open(sys.argv[MAP_FILE_ARG], "r") as filestream:
        for row in filestream:
            rowElement = etree.SubElement(root, "row")
            columns = row.split(" ")

            for column in range(0, len(columns)):
                value = columns[column].replace(",", "").strip()
                columnElement = etree.SubElement(rowElement, "column")

                columnElement.text = value

    with open(OUTPUT_FILENAME, 'wb') as f:
        f.write(etree.tostring(etree.ElementTree(root), pretty_print=True, xml_declaration=True, encoding=ENCODING))

def reindent():
    fileData = None

    with open(OUTPUT_FILENAME, 'r') as file :
        fileData = file.read()

    fileData = fileData.replace(DEFAULT_INDENT, SUPPOSED_INDENT)

    with open(OUTPUT_FILENAME, 'w') as file:
        file.write(fileData)

def main():
    parse()
    reindent()

main()