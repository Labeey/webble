
#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"
#include "http.h"

#define MY_UUID { 0xFE, 0x08, 0x26, 0xE3, 0x0A, 0x24, 0x48, 0xCA, 0xB4, 0x66, 0xDA, 0xE9, 0x75, 0xA4, 0x0A, 0x0B }
PBL_APP_INFO(HTTP_UUID, "Webble", "by Labeey", 1 ,0, /* App version */DEFAULT_MENU_ICON, APP_INFO_STANDARD_APP);


Window window;

TextLayer textLayer;


// Modify these common button handlers

void select_single_click_handler(ClickRecognizerRef recognizer, Window *window) {
	DictionaryIterator* dict;
	http_out_get("http://YOURSITE/wemo/wblink.php", 17, &dict);
	http_out_send();
	text_layer_set_text(&textLayer, "Lights Blinking.");
}

void up_single_click_handler(ClickRecognizerRef recognizer, Window *window) {
  DictionaryIterator* dict;
	http_out_get("http://YOURSITE/wemo/won.php", 17, &dict);
	http_out_send();
	text_layer_set_text(&textLayer, "Lights ON.");

}


void down_single_click_handler(ClickRecognizerRef recognizer, Window *window) {
  DictionaryIterator* dict;
	http_out_get("http://YOURSITE/wemo/woff.php", 17, &dict);
	http_out_send();
	text_layer_set_text(&textLayer, "Lights OFF.");

}

// This usually won't need to be modified

void click_config_provider(ClickConfig **config, Window *window) {
	config[BUTTON_ID_SELECT]->click.handler = (ClickHandler) select_single_click_handler;

        config[BUTTON_ID_UP]->click.handler = (ClickHandler) up_single_click_handler;
        config[BUTTON_ID_UP]->click.repeat_interval_ms = 100;

        config[BUTTON_ID_DOWN]->click.handler = (ClickHandler) down_single_click_handler;
        config[BUTTON_ID_DOWN]->click.repeat_interval_ms = 100;
}

void success(int32_t request_id, int http_status, DictionaryIterator* received, void* context) {
	// Check basic stuff
	if(request_id != 17) {
		text_layer_set_text(&textLayer, "Wrong request_id");
		return;
	}
	if(http_status != 200) {
		text_layer_set_text(&textLayer, "Unexpected http_status");
		return;
	}
	// Check the values.
	Tuple* tuple = dict_find(received, 1);
	if(!tuple) {
		text_layer_set_text(&textLayer, "Webble.");
		return;
	}
	if(tuple->length != 4 || tuple->type != TUPLE_INT) {
		text_layer_set_text(&textLayer, "Key 1 has wrong type.");
		return;
	}
	if(tuple->value->int32 != 42) {
		text_layer_set_text(&textLayer, "Key 1 has wrong value");
		return;
	}
	tuple = dict_find(received, 2);
	if(!tuple) {
		text_layer_set_text(&textLayer, "Key 2 not found.");
		return;
	}
	if(tuple->length != 1 || tuple->type != TUPLE_UINT) {
		text_layer_set_text(&textLayer, "Key 2 has wrong type.");
		return;
	}
	if(tuple->value->uint8 != 18) {
		text_layer_set_text(&textLayer, "Key 2 has wrong value.");
		return;
	}
	tuple = dict_find(received, 3);
	if(!tuple) {
		text_layer_set_text(&textLayer, "Key 3 not found.");
		return;
	}
	if(tuple->type != TUPLE_CSTRING) {
		text_layer_set_text(&textLayer, "Key 3 has wrong type.");
		return;
	}
	text_layer_set_text(&textLayer, tuple->value->cstring);
}

void failure(int32_t request_id, int http_status, void* context) {
	if(request_id == 0) {
		text_layer_set_text(&textLayer, "Request failed with zero request id");
	}
	else if(request_id == 17) {
		text_layer_set_text(&textLayer, "Request failed with expected request id");
	} else {
		text_layer_set_text(&textLayer, "Request failed with unexpected request id");
	}
}

// Standard app initialisation
void handle_init(AppContextRef ctx) {
	window_init(&window, "Webble");
	window_stack_push(&window, true /* Animated */);
	
	text_layer_init(&textLayer, window.layer.frame);
	text_layer_set_text(&textLayer, "Webble");
	text_layer_set_font(&textLayer, fonts_get_system_font(FONT_KEY_GOTHAM_30_BLACK));
	layer_add_child(&window.layer, &textLayer.layer);

	// Attach our desired button functionality
	window_set_click_config_provider(&window, (ClickConfigProvider) click_config_provider);
	
	// Set our app ID
	http_set_app_id(062551715);
	
	// Set up handlers.
	http_register_callbacks((HTTPCallbacks){
		.success = success,
		.failure = failure
	}, NULL);
}


void pbl_main(void *params) {
	PebbleAppHandlers handlers = {
		.init_handler = &handle_init,
		.messaging_info = {
			.buffer_sizes = {
				.inbound = 256,
				.outbound = 256,
			}
		}
	};
	app_event_loop(params, &handlers);
}
