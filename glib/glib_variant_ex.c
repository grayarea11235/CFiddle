#include <glib.h>
#include <glib/gprintf.h>



int main(int argc, char *argv[])
{
  GString *my_string = g_string_new("This Hello world is %d chars long\n");
  g_print(my_string->str, my_string->len);
  g_string_free(my_string, TRUE);

  GVariant *var1;
  GVariant *test_var;
  GArray *test_arr;


  var1 = g_variant_new("y", 200);
  test_var = g_variant_new("(susssasa{sv}i)",
        "Test Application",
        0,
        "nil", // icon
        "title",
        "text <b>bold</b>",
        g_array_new(0, 0, 0), // Action list
        NULL, // hint dict
        10000 // display time ms 
        );
  

  g_variant_unref(var1);

  return 0;
}
