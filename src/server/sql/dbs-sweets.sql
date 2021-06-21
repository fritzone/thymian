-- food list
insert into food(idx, name_source, type, image, food_key) values (99, "donuts_from_transylvania_name", 4, "{#rroot}/img/icon.png", "donuts_from_transylvania");
insert into food(idx, name_source, type, image, food_key) values (100, "apple_cake_with_meringue_name", 4, "{#rroot}/img/icon.png", "apple_cake_with_meringue");
insert into food(idx, name_source, type, image, food_key) values (101, "classic_apple_cake_name", 4, "{#rroot}/img/icon.png", "classic_apple_cake");
insert into food(idx, name_source, type, image, food_key) values (102, "quick_apple_cake_name", 4, "{#rroot}/img/icon.png", "quick_apple_cake");
insert into food(idx, name_source, type, image, food_key) values (103, "lemon_cake_name", 4, "{#rroot}/img/icon.png", "lemon_cake");
insert into food(idx, name_source, type, image, food_key) values (104, "linzer_cookies_name", 4, "{#rroot}/img/icon.png", "linzer_cookies");
insert into food(idx, name_source, type, image, food_key) values (105, "pretzel_name", 4, "{#rroot}/img/icon.png", "pretzel");
insert into food(idx, name_source, type, image, food_key) values (106, "walnut_cake_name", 4, "{#rroot}/img/icon.png", "walnut_cake");
insert into food(idx, name_source, type, image, food_key) values (107, "cottage_cheese_dumplings_name", 4, "{#rroot}/img/icon.png", "cottage_cheese_dumplings");
insert into food(idx, name_source, type, image, food_key) values (108, "plum_dumplings_name", 4, "{#rroot}/img/icon.png", "plum_dumplings");
insert into food(idx, name_source, type, image, food_key) values (109, "pancakes_name", 4, "{#rroot}/img/icon.png", "pancakes");
insert into food(idx, name_source, type, image, food_key) values (110, "fried_pancake_name", 4, "{#rroot}/img/icon.png", "fried_pancake");
insert into food(idx, name_source, type, image, food_key) values (111, "birds_milk_name", 4, "{#rroot}/img/icon.png", "birds_milk");
-- translations for food
insert into translations(source, gb) values ("donuts_from_transylvania_name", "Donuts from Transylvania");
insert into translations(source, gb) values ("apple_cake_with_meringue_name", "Apple cake with meringue");
insert into translations(source, gb) values ("classic_apple_cake_name", "Classic apple cake");
insert into translations(source, gb) values ("quick_apple_cake_name", "Quick apple cake");
insert into translations(source, gb) values ("lemon_cake_name", "Lemon cake");
insert into translations(source, gb) values ("linzer_cookies_name", "Linzer cookies");
insert into translations(source, gb) values ("pretzel_name", "Pretzel");
insert into translations(source, gb) values ("walnut_cake_name", "Walnut cake");
insert into translations(source, gb) values ("cottage_cheese_dumplings_name", "Cottage cheese dumplings");
insert into translations(source, gb) values ("plum_dumplings_name", "Plum dumplings");
insert into translations(source, gb) values ("pancakes_name", "Pancakes");
insert into translations(source, gb) values ("fried_pancake_name", "Fried pancake");
insert into translations(source, gb) values ("birds_milk_name", "Bird's milk");
-- tags of ingredients to food
insert into tags(food_id, ingredient_id) values (100, 4); -- apples / Apple cake with meringue
insert into tags(food_id, ingredient_id) values (100, 60); -- flour / Apple cake with meringue
insert into tags(food_id, ingredient_id) values (100, 21); -- butter / Apple cake with meringue
insert into tags(food_id, ingredient_id) values (100, 86); -- lemon / Apple cake with meringue
insert into tags(food_id, ingredient_id) values (100, 75); -- juice / Apple cake with meringue
insert into tags(food_id, ingredient_id) values (100, 186); -- wine / Apple cake with meringue
insert into tags(food_id, ingredient_id) values (100, 53); -- eggs / Apple cake with meringue
insert into tags(food_id, ingredient_id) values (100, 158); -- sugar / Apple cake with meringue
insert into tags(food_id, ingredient_id) values (100, 74); -- jam / Apple cake with meringue
insert into tags(food_id, ingredient_id) values (100, 39); -- cinnamon / Apple cake with meringue
insert into tags(food_id, ingredient_id) values (101, 60); -- flour / Classic apple cake
insert into tags(food_id, ingredient_id) values (101, 21); -- butter / Classic apple cake
insert into tags(food_id, ingredient_id) values (101, 53); -- eggs / Classic apple cake
insert into tags(food_id, ingredient_id) values (101, 99); -- milk / Classic apple cake
insert into tags(food_id, ingredient_id) values (101, 188); -- yeast / Classic apple cake
insert into tags(food_id, ingredient_id) values (101, 158); -- sugar / Classic apple cake
insert into tags(food_id, ingredient_id) values (101, 4); -- apples / Classic apple cake
insert into tags(food_id, ingredient_id) values (101, 39); -- cinnamon / Classic apple cake
insert into tags(food_id, ingredient_id) values (102, 53); -- eggs / Quick apple cake
insert into tags(food_id, ingredient_id) values (102, 171); -- vanilla / Quick apple cake
insert into tags(food_id, ingredient_id) values (102, 158); -- sugar / Quick apple cake
insert into tags(food_id, ingredient_id) values (102, 60); -- flour / Quick apple cake
insert into tags(food_id, ingredient_id) values (102, 99); -- milk / Quick apple cake
insert into tags(food_id, ingredient_id) values (102, 138); -- salt / Quick apple cake
insert into tags(food_id, ingredient_id) values (102, 86); -- lemon / Quick apple cake
insert into tags(food_id, ingredient_id) values (102, 75); -- juice / Quick apple cake
insert into tags(food_id, ingredient_id) values (102, 3); -- apple / Quick apple cake
insert into tags(food_id, ingredient_id) values (102, 39); -- cinnamon / Quick apple cake
insert into tags(food_id, ingredient_id) values (103, 60); -- flour / Lemon cake
insert into tags(food_id, ingredient_id) values (103, 21); -- butter / Lemon cake
insert into tags(food_id, ingredient_id) values (103, 158); -- sugar / Lemon cake
insert into tags(food_id, ingredient_id) values (103, 99); -- milk / Lemon cake
insert into tags(food_id, ingredient_id) values (103, 52); -- egg / Lemon cake
insert into tags(food_id, ingredient_id) values (103, 86); -- lemon / Lemon cake
insert into tags(food_id, ingredient_id) values (103, 75); -- juice / Lemon cake
insert into tags(food_id, ingredient_id) values (104, 60); -- flour / Linzer cookies
insert into tags(food_id, ingredient_id) values (104, 21); -- butter / Linzer cookies
insert into tags(food_id, ingredient_id) values (104, 158); -- sugar / Linzer cookies
insert into tags(food_id, ingredient_id) values (104, 52); -- egg / Linzer cookies
insert into tags(food_id, ingredient_id) values (104, 86); -- lemon / Linzer cookies
insert into tags(food_id, ingredient_id) values (105, 60); -- flour / Pretzel
insert into tags(food_id, ingredient_id) values (105, 99); -- milk / Pretzel
insert into tags(food_id, ingredient_id) values (105, 188); -- yeast / Pretzel
insert into tags(food_id, ingredient_id) values (105, 158); -- sugar / Pretzel
insert into tags(food_id, ingredient_id) values (105, 138); -- salt / Pretzel
insert into tags(food_id, ingredient_id) values (105, 104); -- oil / Pretzel
insert into tags(food_id, ingredient_id) values (105, 151); -- soda / Pretzel
insert into tags(food_id, ingredient_id) values (105, 52); -- egg / Pretzel
insert into tags(food_id, ingredient_id) values (105, 33); -- cheese / Pretzel
insert into tags(food_id, ingredient_id) values (105, 181); -- water / Pretzel
insert into tags(food_id, ingredient_id) values (106, 60); -- flour / Walnut cake
insert into tags(food_id, ingredient_id) values (106, 21); -- butter / Walnut cake
insert into tags(food_id, ingredient_id) values (106, 158); -- sugar / Walnut cake
insert into tags(food_id, ingredient_id) values (106, 99); -- milk / Walnut cake
insert into tags(food_id, ingredient_id) values (106, 188); -- yeast / Walnut cake
insert into tags(food_id, ingredient_id) values (106, 52); -- egg / Walnut cake
insert into tags(food_id, ingredient_id) values (106, 189); -- yolks / Walnut cake
insert into tags(food_id, ingredient_id) values (106, 86); -- lemon / Walnut cake
insert into tags(food_id, ingredient_id) values (106, 108); -- orange / Walnut cake
insert into tags(food_id, ingredient_id) values (106, 171); -- vanilla / Walnut cake
insert into tags(food_id, ingredient_id) values (107, 33); -- cheese / Cottage cheese dumplings
insert into tags(food_id, ingredient_id) values (107, 53); -- eggs / Cottage cheese dumplings
insert into tags(food_id, ingredient_id) values (107, 21); -- butter / Cottage cheese dumplings
insert into tags(food_id, ingredient_id) values (107, 138); -- salt / Cottage cheese dumplings
insert into tags(food_id, ingredient_id) values (107, 145); -- semolina / Cottage cheese dumplings
insert into tags(food_id, ingredient_id) values (107, 104); -- oil / Cottage cheese dumplings
insert into tags(food_id, ingredient_id) values (107, 18); -- breadcrumbs / Cottage cheese dumplings
insert into tags(food_id, ingredient_id) values (107, 158); -- sugar / Cottage cheese dumplings
insert into tags(food_id, ingredient_id) values (108, 125); -- potatoes / Plum dumplings
insert into tags(food_id, ingredient_id) values (108, 60); -- flour / Plum dumplings
insert into tags(food_id, ingredient_id) values (108, 158); -- sugar / Plum dumplings
insert into tags(food_id, ingredient_id) values (108, 39); -- cinnamon / Plum dumplings
insert into tags(food_id, ingredient_id) values (108, 138); -- salt / Plum dumplings
insert into tags(food_id, ingredient_id) values (108, 21); -- butter / Plum dumplings
insert into tags(food_id, ingredient_id) values (108, 18); -- breadcrumbs / Plum dumplings
insert into tags(food_id, ingredient_id) values (109, 60); -- flour / Pancakes
insert into tags(food_id, ingredient_id) values (109, 53); -- eggs / Pancakes
insert into tags(food_id, ingredient_id) values (109, 138); -- salt / Pancakes
insert into tags(food_id, ingredient_id) values (109, 99); -- milk / Pancakes
insert into tags(food_id, ingredient_id) values (109, 181); -- water / Pancakes
insert into tags(food_id, ingredient_id) values (109, 23); -- cabbage / Pancakes
insert into tags(food_id, ingredient_id) values (109, 152); -- spinach / Pancakes
insert into tags(food_id, ingredient_id) values (109, 33); -- cheese / Pancakes
insert into tags(food_id, ingredient_id) values (109, 52); -- egg / Pancakes
insert into tags(food_id, ingredient_id) values (110, 60); -- flour / Fried pancake
insert into tags(food_id, ingredient_id) values (110, 188); -- yeast / Fried pancake
insert into tags(food_id, ingredient_id) values (110, 52); -- egg / Fried pancake
insert into tags(food_id, ingredient_id) values (110, 99); -- milk / Fried pancake
insert into tags(food_id, ingredient_id) values (110, 33); -- cheese / Fried pancake
insert into tags(food_id, ingredient_id) values (110, 49); -- dill / Fried pancake
insert into tags(food_id, ingredient_id) values (110, 138); -- salt / Fried pancake
insert into tags(food_id, ingredient_id) values (110, 104); -- oil / Fried pancake
insert into tags(food_id, ingredient_id) values (111, 52); -- egg / Bird's milk
insert into tags(food_id, ingredient_id) values (111, 189); -- yolks / Bird's milk
insert into tags(food_id, ingredient_id) values (111, 158); -- sugar / Bird's milk
insert into tags(food_id, ingredient_id) values (111, 99); -- milk / Bird's milk
insert into tags(food_id, ingredient_id) values (111, 171); -- vanilla / Bird's milk
insert into tags(food_id, ingredient_id) values (99, 60); -- flour / Donuts from Transylvania
insert into tags(food_id, ingredient_id) values (99, 158); -- sugar / Donuts from Transylvania
insert into tags(food_id, ingredient_id) values (99, 138); -- salt / Donuts from Transylvania
insert into tags(food_id, ingredient_id) values (99, 52); -- egg / Donuts from Transylvania
insert into tags(food_id, ingredient_id) values (99, 104); -- oil / Donuts from Transylvania
insert into tags(food_id, ingredient_id) values (99, 188); -- yeast / Donuts from Transylvania
insert into tags(food_id, ingredient_id) values (99, 99); -- milk / Donuts from Transylvania
