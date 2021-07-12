import json
import os
import shutil
import re
import sys

import unidecode
import pprint
import string

all_recipe_keys = [ {1 : "furry_bread"}, {2 : "bread_cake"}, {3 : "fried_mushrooms"}, {4 : "jewish_eggs"},
                    {5 : "mushroom_pancake"}, {6 : "mashed_aubergine"}, {7 : "boiled_bacon"}, {8 : "vegetable_spread"},
                    {9 : "homemade_pate"}, {10 : "transylvanian_omelette"}, {11 : "beef_salad"},
                    {12 : "polenta_with_cheese"}, {13 : "clear_chicken_soup"}, {14 : "clear_beef_soup"},
                    {15 : "goulash_soup"}, {16 : "fried_meat_soup"}, {17 : "stew_soup"}, {18 : "sauerkraut_juice_soup"},
                    {19 : "sauerkraut_soup"}, {20 : "cabbage_soup"}, {21 : "stuffed_peppers"}, {22 : "stuffed_leaves"},
                    {23 : "stuffed_squash"}, {24 : "stuffed_kohlrabi"}, {25 : "stuffed_onion"},
                    {26 : "bean_soup_with_tomato"}, {27 : "bean_soup_with_tarragon"}, {28 : "horseradish_soup"},
                    {29 : "potato_soup_with_tarragon"}, {30 : "lamb_soup_with_tarragon"},
                    {31 : "lettuce_soup_with_meat"}, {32 : "bean___lettuce_soup"}, {33 : "lettuce_soup_with_eggs"},
                    {34 : "spinach_soup"}, {35 : "cauliflower_soup"}, {36 : "garlic_soup"}, {37 : "bread_soup"},
                    {38 : "thick_vegetable_soup"}, {39 : "the_fastest_lettuce_soup_ever"}, {40 : "french_bean_soup"},
                    {41 : "egg_soup_from_transylvania"}, {42 : "amazingly_simple_potato_soup"},
                    {43 : "cabbage_tomato_soup"}, {44 : "sour_mushroom_soup"}, {45 : "mushroom_soup"},
                    {46 : "tomato_soup"}, {47 : "tiny_flour_dumpling_soup"}, {48 : "apple_soup"},
                    {49 : "sour_cherry_soup"}, {50 : "dried_prune_soup"}, {52 : "meatball_soup"},
                    {53 : "meatball_soup_with_tarragon"}, {54 : "chicken_soup_with_vegetables"},
                    {55 : "ratatouille_from_transylvania"}, {56 : "gipsy_ratatouille"}, {57 : "potato_goulash"},
                    {58 : "mixed_stew"}, {59 : "risotto_ala_transylvania"}, {60 : "schnitzel"},
                    {61 : "butter_fried_chicken"}, {62 : "meat_roasted_with_onions"}, {63 : "roasted_meat_ala_brasov"},
                    {64 : "meatballs"}, {65 : "smalls"}, {66 : "lamb_cake"}, {67 : "grill_ala_transylvania"},
                    {68 : "chicken_paprikas"}, {69 : "pepper_tokany"}, {70 : "mixed_stew"}, {71 : "mushroom_paprikas"},
                    {72 : "roast_pork_a_la_transylvania"}, {73 : "fried_chicken"}, {74 : "szekely_goulash"},
                    {75 : "layered_cabbage"}, {76 : "stuffed_cabbage"}, {77 : "fried_liver"}, {78 : "layered_potatoes"},
                    {79 : "chulent"}, {80 : "hunters_dish"}, {81 : "chicken_with_carrot"},
                    {82 : "fish_with_vegetables"}, {83 : "fried_potatoes_with_dill"}, {84 : "mashed_potatoes"},
                    {85 : "potato_with_green_parsley"}, {86 : "fried_potato_with_paprika"}, {87 : "pommes_frites"},
                    {88 : "green_peas"}, {89 : "celery_sides"}, {90 : "squash"}, {91 : "green_beans"}, {92 : "spinach"},
                    {93 : "crushed_beans"}, {94 : "cabbage"}, {99 : "donuts_from_transylvania"},
                    {100 : "apple_cake_with_meringue"}, {101 : "classic_apple_cake"}, {102 : "quick_apple_cake"},
                    {103 : "lemon_cake"}, {104 : "linzer_cookies"}, {105 : "pretzel"}, {106 : "walnut_cake"},
                    {107 : "cottage_cheese_dumplings"}, {108 : "plum_dumplings"}, {109 : "pancakes"},
                    {110 : "fried_pancake"}, {111 : "birds_milk"}
                    ]

global_all_ingredients = ['allspice', 'almonds', 'apple', 'apples', 'apricots', 'asparagus', 'aubergine', 'bacon',
                          'baking-powder', 'basil', 'bay-leaves', 'beans', 'beef', 'boneless-chicken', 'bones',
                          'borlotti-beans', 'bread', 'breadcrumbs', 'broth', 'bun', 'butter', 'butterhead-lettuce',
                          'cabbage', 'california-peppers', 'carrot', 'carrots', 'cashews', 'cauliflower',
                          'cayennepepper', 'celery', 'champignon', 'chanterelle', 'cheese', 'cherries', 'chicken',
                          'chicken-breast', 'chilli', 'chorizo', 'cinnamon', 'cloves', 'cod', 'coriander', 'corn',
                          'cottage-cheese', 'crème-fraiche', 'csabai', 'cucumber', 'cumin', 'dill', 'dough', 'duck',
                          'egg', 'eggs', 'entrecote', 'fat', 'feta', 'fettucine', 'fillets', 'fish', 'flour', 'fusilli',
                          'garlic', 'gem-lettuce', 'ginger', 'goose', 'grape-leaves', 'gyulai', 'haricot', 'heart',
                          'herbs', 'hungarian-sausage', 'iceberg-lettuce', 'icing-sugar', 'jam', 'juice', 'kale',
                          'kidney', 'kidney-beans', 'kielbasa', 'knuckle', 'kohlrabi', 'lamb', 'lard', 'lean-meat',
                          'legs', 'lemon', 'lemon-juice', 'lettuce', 'liver', 'lovage', 'lung', 'macaroni', 'marjoram',
                          'marrow', 'marrowbone', 'mayonnaise', 'meat', 'meatballs', 'milk', 'minced-meat', 'mushrooms',
                          'neck', 'nutmeg', 'oil', 'olives', 'onion', 'onions', 'orange', 'oregano', 'oxtail',
                          'oyster-mushrooms', 'paprika', 'parsley', 'parsnips', 'pasta', 'peas', 'penne', 'pepper',
                          'peritoneum', 'pig', 'poppy-seed', 'pork', 'pork-loin', 'potato', 'potatoes', 'prunes',
                          'puree', 'rabbit', 'radish', 'radishes', 'raisins', 'ribs', 'rice', 'romano-lettuce', 'roots',
                          'rosemary', 'roux', 'salt', 'sambal-oelek', 'sauerkraut', 'sauerkraut-juice', 'sausage',
                          'sausages', 'savoury', 'semolina', 'sesame-seed', 'shallots', 'skin', 'smoked', 'smoked-pork',
                          'soda', 'spinach', 'spleen', 'spring-onion', 'spring-onions', 'squashes', 'squash', 'sugar',
                          'sunflower-oil', 'sunflower-seeds', 'tagliatelle', 'tarragon', 'tenderloin', 'thickening',
                          'thyme', 'tomato', 'tomato-juice', 'tomatoes', 'turnip', 'turnips', 'vanilla', 'vanilla-bean',
                          'vanilla-sugar', 'veal', 'vegeta', 'vegetable', 'vegetables', 'vienna', 'vinegar', 'walnut',
                          'water', 'wheat-flour', 'whipped', 'whipped-cream', 'wholemeal-bread', 'wine', 'wings',
                          'yeast', 'yolks', 'zucchini', 'zucchinis']


def ingredient_extractor():
    punc = string.punctuation
    # extract all the ingredients in a separate list
    ing_stid = 1000
    all_ingredients = []
    for r in all_recipes.values():
        for ingrs in r["ingredients"].values():
            for i in ingrs:
                s = list(i.lower())
                ingl = ''.join([o for o in s if not o in punc]).split()
                for ii in ingl:
                    key = ii.lower()
                    if (not re.match(r"[0-9]+", key)) and len(key) > 1 and (key in global_all_ingredients):
                        ing_dict = {"food_id": r["id"], "key": key, "ing_id": ing_stid, "food_name": r["title"],
                                    "dupl": any(d["key"] == key for d in all_ingredients)}
                        ing_stid += 1
                        all_ingredients.append(ing_dict)
    print("-- food list")
    rec_stid = 100
    for r in all_recipes.values():
        new_key = int(r["id"])
        print('insert into food(idx, name_source, type, image, food_key) values ({}, "{}", {}, "{}", "{}");'.format(
            new_key, r["key"] + "_name", rec_stid, "{#rroot}/img/icon.png", r["key"]))
    print("-- translations for food")
    for r in all_recipes.values():
        source = r["key"] + "_name"
        print('insert into translations(source, gb) values ("{}", "{}");'.format(source, r["title"]))
    print("-- tags of ingredients to food")
    all_ingredients = sorted(all_ingredients, key=lambda k: k['food_id'])
    inserted_ingrs = []
    for i in all_ingredients:
        if not i["key"] + str(i["food_id"]) in inserted_ingrs:
            inserted_ingrs.append(i["key"] + str(i["food_id"]))
            print("insert into tags(food_id, ingredient_id) values ({}, {}); -- {} / {}".format(i["food_id"],
                                                                                                global_all_ingredients.index(
                                                                                                    i["key"]) + 1,
                                                                                                i["key"],
                                                                                                i["food_name"]))


def make_key(recipename):
    key = recipename.lower()
    key = unidecode.unidecode(key)
    key = key.replace(" ", "_")
    key = key.replace("-", "_")
    key = key.replace("\"", "")
    key = key.replace("\'", "")
    return key


def process_file(fn):
    global all_recipes
    file1 = open(fn, 'r')
    # contains a recipe mapped to a number, should be the same in all languages
    all_recipes = {}
    count = 0
    # Strips the newline character
    while True:

        current_recipe = {}

        # Get next line from file
        line = file1.readline()

        # end of file is reached
        if not line:
            break

        stripline = line.strip()
        if re.match(r"[0-9]+\.", stripline) or re.match(r"\.[0-9]+\.", stripline) or re.match(r"[0-9][0-9]+\.",
                                                                                              stripline):
            # begin recipe
            # skip line, it's empty
            file1.readline()
            # contains the title
            title = file1.readline().strip()
            if title.endswith("("):
                title = title.replace("(", "").strip()

            current_recipe["id"] = stripline.replace(".", "")
            current_recipe["key"] = make_key(title)
            current_recipe["title"] = title

            # print("{} {}".format(stripline, title) )

            # read until we get ")" as next stripped line
            endoff = False
            while True:
                skipl = file1.readline()
                if not skipl:
                    endoff = True
                    break
                if skipl.strip() == ")":
                    # skip the upcoming empty
                    file1.readline()
                    break

            # end of file?
            if endoff:
                break

            # read in the intro
            intro = ""

            while True:
                skipl = file1.readline()
                if not skipl:
                    endoff = True
                    break
                if skipl == "\n":
                    break
                intro += skipl.strip() + " "

            # end of file?
            if endoff:
                break

            current_recipe["intro"] = intro.strip()
            current_recipe["descr"] = intro.split(".")[0]

            # read in the ingredients and the if you like part
            ingredients = {}
            current_ingredient_part = []
            current_ingredient_key = ""
            while True:
                skipl = file1.readline()
                if not skipl:
                    endoff = True
                    break
                if skipl == "\n":
                    # end of ingredients, fill in the last
                    ingredients[current_ingredient_key] = current_ingredient_part
                    current_ingredient_part = []
                    break
                if skipl.startswith("o "):
                    current_ingredient_part.append(skipl.strip().replace("o ", "", 1).strip())
                else:
                    if not current_ingredient_key:
                        current_ingredient_key = skipl.strip()
                    else:
                        ingredients[current_ingredient_key] = current_ingredient_part
                        current_ingredient_part = []
                        current_ingredient_key = skipl.strip()

            recipe = []
            tips_work = []
            destination = recipe
            # now load the recipe itself, but be careful for tips and remaining ingredients
            while True:
                pos = file1.tell()

                skipl = file1.readline()
                if not skipl:
                    endoff = True
                    break

                trimmed = skipl.strip()

                # did we cross into the next recipe?
                if re.match(r"[0-9]+\.", trimmed) and trimmed:
                    file1.seek(pos)
                    break

                # line number?
                if re.match(r"[0-9]+", trimmed):
                    continue

                # empty line?
                if trimmed == "\n":
                    continue

                # remaining from the ingredients?
                if skipl.startswith("o "):
                    ingredients[current_ingredient_key].append(skipl.strip().replace("o ", "", 1).strip())
                    continue

                if trimmed == "Tips":
                    destination = tips_work

                destination.append(trimmed)

            # fix the tips, so that it does mot contain the . and empty lines
            tips = []
            current_tip = ""
            for tip in tips_work:
                if not tip.strip():
                    continue
                if tip == '•' or tip == "-":
                    if current_tip:
                        tips.append(current_tip)
                        current_tip = ""
                elif tip != "Tips":
                    current_tip += tip + " "
            tips.append(current_tip)

            # fix the recipe, so that it contains only one line, separated by newlines
            current_recipe["fullrecipe"] = " ".join(recipe)
            current_recipe["ingredients"] = ingredients

            all_recipes[current_recipe["id"]] = current_recipe

    return all_recipes


# ingredient_extractor()

files = ["book2-lin-compressed.txt"]

list_all_recipes = []
for file in files:
    list_all_recipes.append(process_file(file))

if os.path.exists("temp_w"):
    shutil.rmtree("temp_w")

if not os.path.exists("temp_w"):
    os.mkdir("temp_w")


for rl in list_all_recipes:
    for r in rl.values():
        # create the directory for the food
        key = ""
        for rec_id_key in all_recipe_keys:
            if int(r["id"]) in rec_id_key:
                key = rec_id_key[int(r["id"])]
                break
        if not key:
            pprint.pprint(r)
            print("No key found for this above")
            sys.exit(3)

        print("update translations set no='{}' where source='{}_name';".format(r["title"], key))
        continue
        wpath = os.path.join("temp_w",  key)
        wpath_no = os.path.join("temp_w",  key, "no")
        if not os.path.exists(wpath):
            os.mkdir(wpath)
            os.mkdir(wpath_no)

        # descr.md
        filepath_descrmd_gb = os.path.join("temp_w", key, "no", "descr.md")
        descr_f = open(filepath_descrmd_gb, "w")
        descr_f.write(r["descr"] + ".")
        descr_f.close()

        # intro.md
        filepath_intromd_gb = os.path.join("temp_w", key, "no", "intro.md")
        intro_f = open(filepath_intromd_gb, "w")
        intro_f.write(r["intro"])
        intro_f.close()

        # recipe.md
        filepath_recipemd_gb = os.path.join("temp_w", key, "no", "recipe.md")
        recipe_f = open(filepath_recipemd_gb, "w")
        ## ingredients
        for ing_root in r["ingredients"].keys():
            recipe_f.write("\n### " + ing_root + "\n")
            for ing in r["ingredients"][ing_root]:
                recipe_f.write("- " + ing + "\n")
        ## the recipe itself
        recipe_f.write("\n### Slik gjør du det\n")

        ### break up the fullrecip ein order to not to have more than two sentenced per paragraph
        full_recipe_text = r["fullrecipe"]
        newline_counter = 0
        chars_written = 0
        newline_needed = False
        for c in full_recipe_text:
            recipe_f.write(c)
            chars_written += 1
            if c == '.':
                newline_counter += 1
                if newline_counter >= 2:
                    if chars_written > 120:
                        newline_counter = 0
                        recipe_f.write("\n\n")

        ## tips
        if "tips" in r:
            recipe_f.write("\nTips\n")
            for tip in r["tips"].values():
                recipe_f.write("- " + tip)
        recipe_f.close()

