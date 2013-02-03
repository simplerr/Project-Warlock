TextMenu = {
	menu_bkgd = "textures/black_transparent.png";
	item_bkgd = "textures/menu_item_bkgd.png";
	font_family = "Arial";
	font_color = "255 0 50 255";
	draw_bkgd = 0;
	centered_items = 0;
	bkgd_scale = 1.2;
	item_bkgd_scale = 1.2;
	pos_x = 500;
	pos_y = 200;
	font_size = 40;
	spacing = 0;
}

ServerMenu = {
	menu_bkgd = "textures/white_transparent.png";
	item_bkgd = "textures/menu_item_bkgd.png";
	font_family = "Consolas";
	font_color = "0 0 0 255";
	draw_bkgd = 1;
	draw_item_bkgd = 0;
	centered_items = 0;
	bkgd_scale = 1.1;
	item_bkgd_scale = 1.0;
	pos_x = 800;
	pos_y = 200;
	font_size = 30;
	spacing = 0;
}

-- Titles

StateHeader = {
	pos_x = 800;
	pos_y = 50;
	font_size = 60;
	font_family = "Arial";
	font_color = "0 0 0 255";
}

-- Buttons

DefaultButtonData = {
	font_size = 30;
	font_family = "Arial";
	font_color = "255 255 255 255";
	default_texture = "textures/button_default.png";
	disabled_texture = "textures/button_disabled.png";
	hoover_texture = "textures/button_hoover.png";
}

StartGameButton = {
	pos_x = 1500;
	pos_y = 50;
	font_size = DefaultButtonData.font_size;
	font_family = DefaultButtonData.font_family;
	font_color = DefaultButtonData.font_color;
	default_texture = DefaultButtonData.default_texture;
	disabled_texture = DefaultButtonData.disabled_texture;
	hoover_texture = DefaultButtonData.hoover_texture;
}

LeaveLobbyButton = {
	pos_x = 1500;
	pos_y = 100;
	font_size = DefaultButtonData.font_size;
	font_family = DefaultButtonData.font_family;
	font_color = DefaultButtonData.font_color;
	default_texture = DefaultButtonData.default_texture;
	disabled_texture = DefaultButtonData.disabled_texture;
	hoover_texture = DefaultButtonData.hoover_texture;
}