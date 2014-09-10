import re

html = """<!doctype html>
<html>
    <head>
        <meta http-equiv="content-type" content="text/html; charset=utf-8" />
        <title>A Midsummer Night's Dream</title>
    </head>
    <body>"""

# The link format for character names
name_spec = "       <p><a href=\"characters.html#{1}\">{0}</a><br />\n"

act_scene_declaration = "Act \d+, Scene \d+"
act_scene_declaration = re.compile(act_scene_declaration)

# The format for regular lines
line_spec = "       {0}<br />\n"

character_names = ["THESEUS", "PHILOSTRATE", "LYSANDER", "HIPPOLYTA", "DEMETRIUS",
                    "PUCK", "OBERON", "TITANIA"]
with open("files/dream.txt") as play_text:
    for line in play_text:
        line = line.strip()
        if line in character_names:
            html += name_spec.format(line, line.lower())
        elif re.match(act_scene_declaration, line) is not None:
            html += "<h2>{0}</h2>".format(line)
        elif line == "A Midsummer Night's Dream":
            html += "<h1>{0}</h1>".format(line)
        else:
            html += line_spec.format(line)




html += """     </p>
    </body>
</html>"""

print html
