-- food list
insert into food(idx, name_source, type, image, food_key) values (1, "furry_bread_name", 100, "{#rroot}/img/icon.png", "furry_bread");
insert into food(idx, name_source, type, image, food_key) values (2, "bread_cake_name", 100, "{#rroot}/img/icon.png", "bread_cake");
insert into food(idx, name_source, type, image, food_key) values (3, "fried_mushrooms_name", 100, "{#rroot}/img/icon.png", "fried_mushrooms");
insert into food(idx, name_source, type, image, food_key) values (4, "jewish_eggs_name", 100, "{#rroot}/img/icon.png", "jewish_eggs");
insert into food(idx, name_source, type, image, food_key) values (5, "mushroom_pancake_name", 100, "{#rroot}/img/icon.png", "mushroom_pancake");
insert into food(idx, name_source, type, image, food_key) values (6, "mashed_aubergine_name", 100, "{#rroot}/img/icon.png", "mashed_aubergine");
insert into food(idx, name_source, type, image, food_key) values (7, "boiled_bacon_name", 100, "{#rroot}/img/icon.png", "boiled_bacon");
insert into food(idx, name_source, type, image, food_key) values (8, "vegetable_spread_name", 100, "{#rroot}/img/icon.png", "vegetable_spread");
insert into food(idx, name_source, type, image, food_key) values (9, "homemade_pate_name", 100, "{#rroot}/img/icon.png", "homemade_pate");
insert into food(idx, name_source, type, image, food_key) values (10, "transylvanian_omelette_name", 100, "{#rroot}/img/icon.png", "transylvanian_omelette");
insert into food(idx, name_source, type, image, food_key) values (11, "beef_salad_name", 100, "{#rroot}/img/icon.png", "beef_salad");
insert into food(idx, name_source, type, image, food_key) values (12, "polenta_with_cheese_name", 100, "{#rroot}/img/icon.png", "polenta_with_cheese");
-- translations for food
insert into translations(source, gb) values ("furry_bread_name", "Furry bread");
insert into translations(source, gb) values ("bread_cake_name", "Bread-cake");
insert into translations(source, gb) values ("fried_mushrooms_name", "Fried mushrooms");
insert into translations(source, gb) values ("jewish_eggs_name", "Jewish eggs");
insert into translations(source, gb) values ("mushroom_pancake_name", "Mushroom pancake");
insert into translations(source, gb) values ("mashed_aubergine_name", "Mashed aubergine");
insert into translations(source, gb) values ("boiled_bacon_name", "Boiled bacon");
insert into translations(source, gb) values ("vegetable_spread_name", "Vegetable spread");
insert into translations(source, gb) values ("homemade_pate_name", "Homemade Pâté");
insert into translations(source, gb) values ("transylvanian_omelette_name", "Transylvanian omelette");
insert into translations(source, gb) values ("beef_salad_name", "Beef salad");
insert into translations(source, gb) values ("polenta_with_cheese_name", "Polenta with cheese");
-- tags of ingredients to food
insert into tags(food_id, ingredient_id) values (1, 17); -- bread / Furry bread
insert into tags(food_id, ingredient_id) values (1, 53); -- eggs / Furry bread
insert into tags(food_id, ingredient_id) values (1, 99); -- milk / Furry bread
insert into tags(food_id, ingredient_id) values (1, 138); -- salt / Furry bread
insert into tags(food_id, ingredient_id) values (1, 118); -- pepper / Furry bread
insert into tags(food_id, ingredient_id) values (1, 104); -- oil / Furry bread
insert into tags(food_id, ingredient_id) values (1, 74); -- jam / Furry bread
insert into tags(food_id, ingredient_id) values (1, 33); -- cheese / Furry bread
insert into tags(food_id, ingredient_id) values (1, 29); -- cayennepepper / Furry bread
insert into tags(food_id, ingredient_id) values (10, 53); -- eggs / Transylvanian omelette
insert into tags(food_id, ingredient_id) values (10, 8); -- bacon / Transylvanian omelette
insert into tags(food_id, ingredient_id) values (10, 104); -- oil / Transylvanian omelette
insert into tags(food_id, ingredient_id) values (10, 83); -- lard / Transylvanian omelette
insert into tags(food_id, ingredient_id) values (10, 138); -- salt / Transylvanian omelette
insert into tags(food_id, ingredient_id) values (10, 17); -- bread / Transylvanian omelette
insert into tags(food_id, ingredient_id) values (10, 177); -- vegetables / Transylvanian omelette
insert into tags(food_id, ingredient_id) values (10, 118); -- pepper / Transylvanian omelette
insert into tags(food_id, ingredient_id) values (10, 107); -- onions / Transylvanian omelette
insert into tags(food_id, ingredient_id) values (10, 101); -- mushrooms / Transylvanian omelette
insert into tags(food_id, ingredient_id) values (10, 33); -- cheese / Transylvanian omelette
insert into tags(food_id, ingredient_id) values (11, 26); -- carrots / Beef salad
insert into tags(food_id, ingredient_id) values (11, 114); -- parsnips / Beef salad
insert into tags(food_id, ingredient_id) values (11, 30); -- celery / Beef salad
insert into tags(food_id, ingredient_id) values (11, 113); -- parsley / Beef salad
insert into tags(food_id, ingredient_id) values (11, 125); -- potatoes / Beef salad
insert into tags(food_id, ingredient_id) values (11, 116); -- peas / Beef salad
insert into tags(food_id, ingredient_id) values (11, 12); -- beans / Beef salad
insert into tags(food_id, ingredient_id) values (11, 170); -- turnips / Beef salad
insert into tags(food_id, ingredient_id) values (11, 47); -- cucumber / Beef salad
insert into tags(food_id, ingredient_id) values (11, 105); -- olives / Beef salad
insert into tags(food_id, ingredient_id) values (11, 97); -- meat / Beef salad
insert into tags(food_id, ingredient_id) values (11, 13); -- beef / Beef salad
insert into tags(food_id, ingredient_id) values (11, 96); -- mayonnaise / Beef salad
insert into tags(food_id, ingredient_id) values (12, 43); -- corn / Polenta with cheese
insert into tags(food_id, ingredient_id) values (12, 60); -- flour / Polenta with cheese
insert into tags(food_id, ingredient_id) values (12, 138); -- salt / Polenta with cheese
insert into tags(food_id, ingredient_id) values (12, 33); -- cheese / Polenta with cheese
insert into tags(food_id, ingredient_id) values (12, 181); -- water / Polenta with cheese
insert into tags(food_id, ingredient_id) values (2, 17); -- bread / Bread-cake
insert into tags(food_id, ingredient_id) values (2, 8); -- bacon / Bread-cake
insert into tags(food_id, ingredient_id) values (2, 52); -- egg / Bread-cake
insert into tags(food_id, ingredient_id) values (2, 99); -- milk / Bread-cake
insert into tags(food_id, ingredient_id) values (2, 33); -- cheese / Bread-cake
insert into tags(food_id, ingredient_id) values (2, 138); -- salt / Bread-cake
insert into tags(food_id, ingredient_id) values (2, 118); -- pepper / Bread-cake
insert into tags(food_id, ingredient_id) values (2, 106); -- onion / Bread-cake
insert into tags(food_id, ingredient_id) values (2, 105); -- olives / Bread-cake
insert into tags(food_id, ingredient_id) values (2, 113); -- parsley / Bread-cake
insert into tags(food_id, ingredient_id) values (3, 101); -- mushrooms / Fried mushrooms
insert into tags(food_id, ingredient_id) values (3, 107); -- onions / Fried mushrooms
insert into tags(food_id, ingredient_id) values (3, 62); -- garlic / Fried mushrooms
insert into tags(food_id, ingredient_id) values (3, 40); -- cloves / Fried mushrooms
insert into tags(food_id, ingredient_id) values (3, 21); -- butter / Fried mushrooms
insert into tags(food_id, ingredient_id) values (3, 138); -- salt / Fried mushrooms
insert into tags(food_id, ingredient_id) values (3, 118); -- pepper / Fried mushrooms
insert into tags(food_id, ingredient_id) values (4, 53); -- eggs / Jewish eggs
insert into tags(food_id, ingredient_id) values (4, 106); -- onion / Jewish eggs
insert into tags(food_id, ingredient_id) values (4, 138); -- salt / Jewish eggs
insert into tags(food_id, ingredient_id) values (4, 118); -- pepper / Jewish eggs
insert into tags(food_id, ingredient_id) values (4, 104); -- oil / Jewish eggs
insert into tags(food_id, ingredient_id) values (4, 65); -- goose / Jewish eggs
insert into tags(food_id, ingredient_id) values (4, 51); -- duck / Jewish eggs
insert into tags(food_id, ingredient_id) values (4, 89); -- liver / Jewish eggs
insert into tags(food_id, ingredient_id) values (5, 106); -- onion / Mushroom pancake
insert into tags(food_id, ingredient_id) values (5, 101); -- mushrooms / Mushroom pancake
insert into tags(food_id, ingredient_id) values (5, 52); -- egg / Mushroom pancake
insert into tags(food_id, ingredient_id) values (5, 21); -- butter / Mushroom pancake
insert into tags(food_id, ingredient_id) values (5, 60); -- flour / Mushroom pancake
insert into tags(food_id, ingredient_id) values (5, 104); -- oil / Mushroom pancake
insert into tags(food_id, ingredient_id) values (5, 138); -- salt / Mushroom pancake
insert into tags(food_id, ingredient_id) values (5, 118); -- pepper / Mushroom pancake
insert into tags(food_id, ingredient_id) values (5, 112); -- paprika / Mushroom pancake
insert into tags(food_id, ingredient_id) values (5, 113); -- parsley / Mushroom pancake
insert into tags(food_id, ingredient_id) values (5, 179); -- vinegar / Mushroom pancake
insert into tags(food_id, ingredient_id) values (5, 99); -- milk / Mushroom pancake
insert into tags(food_id, ingredient_id) values (6, 7); -- aubergine / Mashed aubergine
insert into tags(food_id, ingredient_id) values (6, 104); -- oil / Mashed aubergine
insert into tags(food_id, ingredient_id) values (6, 138); -- salt / Mashed aubergine
insert into tags(food_id, ingredient_id) values (6, 106); -- onion / Mashed aubergine
insert into tags(food_id, ingredient_id) values (6, 96); -- mayonnaise / Mashed aubergine
insert into tags(food_id, ingredient_id) values (6, 62); -- garlic / Mashed aubergine
insert into tags(food_id, ingredient_id) values (6, 40); -- cloves / Mashed aubergine
insert into tags(food_id, ingredient_id) values (6, 75); -- juice / Mashed aubergine
insert into tags(food_id, ingredient_id) values (6, 86); -- lemon / Mashed aubergine
insert into tags(food_id, ingredient_id) values (7, 8); -- bacon / Boiled bacon
insert into tags(food_id, ingredient_id) values (7, 62); -- garlic / Boiled bacon
insert into tags(food_id, ingredient_id) values (7, 40); -- cloves / Boiled bacon
insert into tags(food_id, ingredient_id) values (7, 112); -- paprika / Boiled bacon
insert into tags(food_id, ingredient_id) values (7, 138); -- salt / Boiled bacon
insert into tags(food_id, ingredient_id) values (7, 118); -- pepper / Boiled bacon
insert into tags(food_id, ingredient_id) values (8, 106); -- onion / Vegetable spread
insert into tags(food_id, ingredient_id) values (8, 168); -- tomatoes / Vegetable spread
insert into tags(food_id, ingredient_id) values (8, 12); -- beans / Vegetable spread
insert into tags(food_id, ingredient_id) values (8, 118); -- pepper / Vegetable spread
insert into tags(food_id, ingredient_id) values (8, 138); -- salt / Vegetable spread
insert into tags(food_id, ingredient_id) values (8, 104); -- oil / Vegetable spread
insert into tags(food_id, ingredient_id) values (9, 122); -- pork / Homemade Pâté
insert into tags(food_id, ingredient_id) values (9, 89); -- liver / Homemade Pâté
insert into tags(food_id, ingredient_id) values (9, 97); -- meat / Homemade Pâté
insert into tags(food_id, ingredient_id) values (9, 102); -- neck / Homemade Pâté
insert into tags(food_id, ingredient_id) values (9, 15); -- bones / Homemade Pâté
insert into tags(food_id, ingredient_id) values (9, 55); -- fat / Homemade Pâté
insert into tags(food_id, ingredient_id) values (9, 8); -- bacon / Homemade Pâté
insert into tags(food_id, ingredient_id) values (9, 107); -- onions / Homemade Pâté
insert into tags(food_id, ingredient_id) values (9, 1); -- allspice / Homemade Pâté
insert into tags(food_id, ingredient_id) values (9, 103); -- nutmeg / Homemade Pâté
insert into tags(food_id, ingredient_id) values (9, 138); -- salt / Homemade Pâté
insert into tags(food_id, ingredient_id) values (9, 118); -- pepper / Homemade Pâté
insert into tags(food_id, ingredient_id) values (9, 104); -- oil / Homemade Pâté
