-- food list
insert into food(idx, name_source, type, image, food_key) values (83, "fried_potatoes_with_dill_name", 2, "{#rroot}/img/icon.png", "fried_potatoes_with_dill");
insert into food(idx, name_source, type, image, food_key) values (84, "mashed_potatoes_name", 2, "{#rroot}/img/icon.png", "mashed_potatoes");
insert into food(idx, name_source, type, image, food_key) values (85, "potato_with_green_parsley_name", 2, "{#rroot}/img/icon.png", "potato_with_green_parsley");
insert into food(idx, name_source, type, image, food_key) values (86, "fried_potato_with_paprika_name", 2, "{#rroot}/img/icon.png", "fried_potato_with_paprika");
insert into food(idx, name_source, type, image, food_key) values (87, "pommes_frites_name", 2, "{#rroot}/img/icon.png", "pommes_frites");
insert into food(idx, name_source, type, image, food_key) values (88, "green_peas_name", 2, "{#rroot}/img/icon.png", "green_peas");
insert into food(idx, name_source, type, image, food_key) values (89, "celery_sides_name", 2, "{#rroot}/img/icon.png", "celery_sides");
insert into food(idx, name_source, type, image, food_key) values (90, "squash_name", 2, "{#rroot}/img/icon.png", "squash");
insert into food(idx, name_source, type, image, food_key) values (91, "green_beans_name", 2, "{#rroot}/img/icon.png", "green_beans");
insert into food(idx, name_source, type, image, food_key) values (92, "spinach_name", 2, "{#rroot}/img/icon.png", "spinach");
insert into food(idx, name_source, type, image, food_key) values (93, "crushed_beans_name", 2, "{#rroot}/img/icon.png", "crushed_beans");
insert into food(idx, name_source, type, image, food_key) values (94, "cabbage_name", 2, "{#rroot}/img/icon.png", "cabbage");
-- translations for food
insert into translations(source, gb) values ("fried_potatoes_with_dill_name", "Fried potatoes with dill");
insert into translations(source, gb) values ("mashed_potatoes_name", "Mashed potatoes");
insert into translations(source, gb) values ("potato_with_green_parsley_name", "Potato with green parsley");
insert into translations(source, gb) values ("fried_potato_with_paprika_name", "Fried potato with paprika");
insert into translations(source, gb) values ("pommes_frites_name", "Pommes frites");
insert into translations(source, gb) values ("green_peas_name", "Green peas");
insert into translations(source, gb) values ("celery_sides_name", "Celery sides");
insert into translations(source, gb) values ("squash_name", "Squash");
insert into translations(source, gb) values ("green_beans_name", "Green beans");
insert into translations(source, gb) values ("spinach_name", "Spinach");
insert into translations(source, gb) values ("crushed_beans_name", "Crushed beans");
insert into translations(source, gb) values ("cabbage_name", "Cabbage");
-- tags of ingredients to food
insert into tags(food_id, ingredient_id) values (83, 125); -- potatoes / Fried potatoes with dill
insert into tags(food_id, ingredient_id) values (83, 21); -- butter / Fried potatoes with dill
insert into tags(food_id, ingredient_id) values (83, 138); -- salt / Fried potatoes with dill
insert into tags(food_id, ingredient_id) values (83, 118); -- pepper / Fried potatoes with dill
insert into tags(food_id, ingredient_id) values (83, 49); -- dill / Fried potatoes with dill
insert into tags(food_id, ingredient_id) values (84, 125); -- potatoes / Mashed potatoes
insert into tags(food_id, ingredient_id) values (84, 138); -- salt / Mashed potatoes
insert into tags(food_id, ingredient_id) values (84, 99); -- milk / Mashed potatoes
insert into tags(food_id, ingredient_id) values (84, 21); -- butter / Mashed potatoes
insert into tags(food_id, ingredient_id) values (84, 175); -- vegeta / Mashed potatoes
insert into tags(food_id, ingredient_id) values (84, 104); -- oil / Mashed potatoes
insert into tags(food_id, ingredient_id) values (85, 125); -- potatoes / Potato with green parsley
insert into tags(food_id, ingredient_id) values (85, 21); -- butter / Potato with green parsley
insert into tags(food_id, ingredient_id) values (85, 113); -- parsley / Potato with green parsley
insert into tags(food_id, ingredient_id) values (85, 138); -- salt / Potato with green parsley
insert into tags(food_id, ingredient_id) values (86, 106); -- onion / Fried potato with paprika
insert into tags(food_id, ingredient_id) values (86, 125); -- potatoes / Fried potato with paprika
insert into tags(food_id, ingredient_id) values (86, 112); -- paprika / Fried potato with paprika
insert into tags(food_id, ingredient_id) values (86, 104); -- oil / Fried potato with paprika
insert into tags(food_id, ingredient_id) values (87, 125); -- potatoes / Pommes frites
insert into tags(food_id, ingredient_id) values (87, 104); -- oil / Pommes frites
insert into tags(food_id, ingredient_id) values (88, 116); -- peas / Green peas
insert into tags(food_id, ingredient_id) values (88, 104); -- oil / Green peas
insert into tags(food_id, ingredient_id) values (88, 60); -- flour / Green peas
insert into tags(food_id, ingredient_id) values (88, 137); -- roux / Green peas
insert into tags(food_id, ingredient_id) values (88, 113); -- parsley / Green peas
insert into tags(food_id, ingredient_id) values (88, 138); -- salt / Green peas
insert into tags(food_id, ingredient_id) values (88, 158); -- sugar / Green peas
insert into tags(food_id, ingredient_id) values (88, 179); -- vinegar / Green peas
insert into tags(food_id, ingredient_id) values (88, 166); -- tomato / Green peas
insert into tags(food_id, ingredient_id) values (88, 21); -- butter / Green peas
insert into tags(food_id, ingredient_id) values (89, 30); -- celery / Celery sides
insert into tags(food_id, ingredient_id) values (89, 21); -- butter / Celery sides
insert into tags(food_id, ingredient_id) values (89, 138); -- salt / Celery sides
insert into tags(food_id, ingredient_id) values (89, 162); -- tarragon / Celery sides
insert into tags(food_id, ingredient_id) values (89, 113); -- parsley / Celery sides
insert into tags(food_id, ingredient_id) values (90, 156); -- squashes / Squash
insert into tags(food_id, ingredient_id) values (90, 191); -- zucchinis / Squash
insert into tags(food_id, ingredient_id) values (90, 49); -- dill / Squash
insert into tags(food_id, ingredient_id) values (90, 109); -- oregano / Squash
insert into tags(food_id, ingredient_id) values (90, 179); -- vinegar / Squash
insert into tags(food_id, ingredient_id) values (90, 60); -- flour / Squash
insert into tags(food_id, ingredient_id) values (90, 104); -- oil / Squash
insert into tags(food_id, ingredient_id) values (90, 138); -- salt / Squash
insert into tags(food_id, ingredient_id) values (90, 112); -- paprika / Squash
insert into tags(food_id, ingredient_id) values (91, 12); -- beans / Green beans
insert into tags(food_id, ingredient_id) values (91, 106); -- onion / Green beans
insert into tags(food_id, ingredient_id) values (91, 168); -- tomatoes / Green beans
insert into tags(food_id, ingredient_id) values (91, 113); -- parsley / Green beans
insert into tags(food_id, ingredient_id) values (91, 138); -- salt / Green beans
insert into tags(food_id, ingredient_id) values (91, 158); -- sugar / Green beans
insert into tags(food_id, ingredient_id) values (91, 179); -- vinegar / Green beans
insert into tags(food_id, ingredient_id) values (91, 60); -- flour / Green beans
insert into tags(food_id, ingredient_id) values (91, 104); -- oil / Green beans
insert into tags(food_id, ingredient_id) values (91, 62); -- garlic / Green beans
insert into tags(food_id, ingredient_id) values (91, 40); -- cloves / Green beans
insert into tags(food_id, ingredient_id) values (91, 112); -- paprika / Green beans
insert into tags(food_id, ingredient_id) values (92, 152); -- spinach / Spinach
insert into tags(food_id, ingredient_id) values (92, 62); -- garlic / Spinach
insert into tags(food_id, ingredient_id) values (92, 40); -- cloves / Spinach
insert into tags(food_id, ingredient_id) values (92, 99); -- milk / Spinach
insert into tags(food_id, ingredient_id) values (92, 21); -- butter / Spinach
insert into tags(food_id, ingredient_id) values (92, 104); -- oil / Spinach
insert into tags(food_id, ingredient_id) values (92, 60); -- flour / Spinach
insert into tags(food_id, ingredient_id) values (92, 138); -- salt / Spinach
insert into tags(food_id, ingredient_id) values (93, 12); -- beans / Crushed beans
insert into tags(food_id, ingredient_id) values (93, 106); -- onion / Crushed beans
insert into tags(food_id, ingredient_id) values (93, 8); -- bacon / Crushed beans
insert into tags(food_id, ingredient_id) values (93, 104); -- oil / Crushed beans
insert into tags(food_id, ingredient_id) values (93, 60); -- flour / Crushed beans
insert into tags(food_id, ingredient_id) values (93, 138); -- salt / Crushed beans
insert into tags(food_id, ingredient_id) values (93, 118); -- pepper / Crushed beans
insert into tags(food_id, ingredient_id) values (93, 179); -- vinegar / Crushed beans
insert into tags(food_id, ingredient_id) values (93, 113); -- parsley / Crushed beans
insert into tags(food_id, ingredient_id) values (94, 23); -- cabbage / Cabbage
insert into tags(food_id, ingredient_id) values (94, 106); -- onion / Cabbage
insert into tags(food_id, ingredient_id) values (94, 60); -- flour / Cabbage
insert into tags(food_id, ingredient_id) values (94, 104); -- oil / Cabbage
insert into tags(food_id, ingredient_id) values (94, 138); -- salt / Cabbage
insert into tags(food_id, ingredient_id) values (94, 118); -- pepper / Cabbage
insert into tags(food_id, ingredient_id) values (94, 112); -- paprika / Cabbage
insert into tags(food_id, ingredient_id) values (94, 109); -- oregano / Cabbage
insert into tags(food_id, ingredient_id) values (94, 49); -- dill / Cabbage
