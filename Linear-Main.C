#include <pebble.h>
  
static Window *s_main_window;
static TextLayer *s_hour_layer;
static TextLayer *s_min_layer;
static TextLayer *s_month_layer;
static TextLayer *s_plus_min_layer;
static TextLayer *s_background;
static TextLayer *s_plus_hour_layer;

static void update_time() {
  // Get a tm structure
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);

  // Create a long-lived buffer
  static char buffer[] = "00";
  static char buffer_min[] = "00";
  static char buffer_month[80];
  static char buffer_plus_min[80];
  static int real_min=0;
  //static char pm_min[4]; 
  static char buffer_plus_hour[80];
  static int real_hour=0;
  
  // Write the current hours and minutes into the buffer
  if(clock_is_24h_style() == true) {
    //Use 2h hour format
    strftime(buffer, 80, "%H", tick_time);
  } else {
    //Use 12 hour format
    strftime(buffer, 80, "%I", tick_time);
  }
  strftime(buffer_min, 80, "%M", tick_time);  
  strftime(buffer_month, 80, "%d %b", tick_time);
  
  real_hour = (buffer[0]-'0')*10; //is this broken with numbers less than 10?
  real_hour = (real_hour +(buffer[1]-'0'));
  real_min = (buffer_min[0]-'0') * 10;
  real_min = (real_min +(buffer_min[1]-'0'));
  
  strcpy(buffer_plus_hour,"");
  strcpy(buffer_plus_min,"");

  for (int i =1;i<=4;i++){
    char holder_array[80];
    char holder_array_min[80];
    int holder_num = 0;
    int holder_min = 0;
    if (i >2){
      holder_num = (real_hour + i)-2;
      holder_min = (real_min + i)-2;
    }else{
      holder_num = real_hour - (3-i);
      holder_min = real_min - (3-i);
    }
    if (holder_num <10){
      snprintf(holder_array, sizeof(buffer_plus_hour),"0%d ", holder_num);
    }else{
      snprintf(holder_array, sizeof(buffer_plus_hour),"%d ", holder_num);
    }
    if (holder_min<10){
      snprintf(holder_array_min, sizeof(buffer_plus_min),"0%d ", holder_min);
    }else{
      snprintf(holder_array_min, sizeof(buffer_plus_min),"%d ", holder_min);
    }
    strcat(buffer_plus_hour, holder_array); 
    strcat(buffer_plus_min, holder_array_min); 
    if (i == 2){
      strcat(buffer_plus_hour,"        ");
      strcat(buffer_plus_min,"        ");
    }
  }

  // Display this time on the TextLayer
  text_layer_set_text(s_background, "");
  text_layer_set_text(s_month_layer, buffer_month);
  text_layer_set_text(s_plus_hour_layer,buffer_plus_hour);
  text_layer_set_text(s_hour_layer, buffer);
  text_layer_set_text(s_plus_min_layer, buffer_plus_min);
  text_layer_set_text(s_min_layer, buffer_min);
}

static void main_window_load(Window *window) {
  s_background = text_layer_create(GRect(0, 0, 144, 175));
  text_layer_set_background_color(s_background, GColorBlack); 
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_background));

  // Create time TextLayer
  s_plus_hour_layer = text_layer_create(GRect(0, 40, 144, 50));
  text_layer_set_background_color(s_plus_hour_layer, GColorClear);
  text_layer_set_text_color(s_plus_hour_layer, GColorClear);
  text_layer_set_text(s_plus_hour_layer, "00");
  // Improve the layout to be more like a watchface
  text_layer_set_font(s_plus_hour_layer, fonts_get_system_font(FONT_KEY_ROBOTO_CONDENSED_21));
  text_layer_set_text_alignment(s_plus_hour_layer, GTextAlignmentCenter);
  // Add it as a child layer to the Window's root layer
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_plus_hour_layer));  
  
  // Create time TextLayer
  s_hour_layer = text_layer_create(GRect(0, 15, 144, 50));
  text_layer_set_background_color(s_hour_layer, GColorClear);
  text_layer_set_text_color(s_hour_layer, GColorClear);
  text_layer_set_text(s_hour_layer, "00");
  // Improve the layout to be more like a watchface
  text_layer_set_font(s_hour_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_MEDIUM_NUMBERS));
  text_layer_set_text_alignment(s_hour_layer, GTextAlignmentCenter);
  // Add it as a child layer to the Window's root layer
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_hour_layer));
  
  // Create minute TextLayer
  s_plus_min_layer = text_layer_create(GRect(0, 90, 144, 50));
  text_layer_set_background_color(s_plus_min_layer, GColorClear);
  text_layer_set_text_color(s_plus_min_layer, GColorClear);
  text_layer_set_text(s_plus_min_layer, "00");
  // Improve the layout to be more like a watchface
  text_layer_set_font(s_plus_min_layer, fonts_get_system_font(FONT_KEY_ROBOTO_CONDENSED_21));
  text_layer_set_text_alignment(s_plus_min_layer, GTextAlignmentLeft);
  // Add it as a child layer to the Window's root layer
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_plus_min_layer));
  
    // Create minute TextLayer
  s_min_layer = text_layer_create(GRect(0, 75, 144, 50));
  text_layer_set_background_color(s_min_layer, GColorClear);
  text_layer_set_text_color(s_min_layer, GColorClear);
  text_layer_set_text(s_min_layer, "00");
  // Improve the layout to be more like a watchface
  text_layer_set_font(s_min_layer, fonts_get_system_font(FONT_KEY_BITHAM_34_MEDIUM_NUMBERS));
  text_layer_set_text_alignment(s_min_layer, GTextAlignmentCenter);
  // Add it as a child layer to the Window's root layer
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_min_layer));
  
  // Create minute TextLayer
  s_month_layer = text_layer_create(GRect(0, 120, 144, 50));
  text_layer_set_background_color(s_month_layer, GColorClear);
  text_layer_set_text_color(s_month_layer, GColorClear);
  text_layer_set_text(s_month_layer, "00");
  // Improve the layout to be more like a watchface
  text_layer_set_font(s_month_layer, fonts_get_system_font(FONT_KEY_ROBOTO_CONDENSED_21));
  text_layer_set_text_alignment(s_month_layer, GTextAlignmentCenter);
  // Add it as a child layer to the Window's root layer
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_month_layer));
  
  
  // Make sure the time is displayed from the start
  update_time();
}

static void main_window_unload(Window *window) {
  // Destroy TextLayer
  text_layer_destroy(s_hour_layer);
  text_layer_destroy(s_plus_hour_layer);
  text_layer_destroy(s_min_layer);
  text_layer_destroy(s_month_layer);
  text_layer_destroy(s_plus_min_layer);
  text_layer_destroy(s_background);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}
  
static void init() {
  // Create main Window element and assign to pointer
  s_main_window = window_create();

  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });

  // Show the Window on the watch, with animated=true
  window_stack_push(s_main_window, true);
  
  // Register with TickTimerService
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
}

static void deinit() {
  // Destroy Window
  window_destroy(s_main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
