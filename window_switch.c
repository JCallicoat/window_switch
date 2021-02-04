// Simple program to switch x11 windows on the active workspace
//
// MIT License, 2020 Jordan Callicoat <jordan.callicoat@gmail.com>

#include <gdk/gdkx.h>
#include <libwnck/libwnck.h>

int main(int argc, char **argv) {
  WnckScreen *screen;
  WnckWindow *active_window;
  WnckWorkspace *workspace;
  GList *all_windows;
  GList *workspace_windows;
  WnckWindow *window;

  gdk_init(&argc, &argv);

  screen = wnck_screen_get_default();
  wnck_screen_force_update(screen); // populate screen immediately

  active_window = wnck_screen_get_active_window(screen);
  workspace = wnck_window_get_workspace(active_window);

  // filter windows by active workspace and window type to ignore dock and
  // desktop windows etc
  for (all_windows = wnck_screen_get_windows(screen); all_windows != NULL;
       all_windows = all_windows->next) {
    window = WNCK_WINDOW(all_windows->data);
    if (wnck_window_is_on_workspace(window, workspace) &&
        wnck_window_get_window_type(window) == WNCK_WINDOW_NORMAL) {
      workspace_windows = g_list_append(workspace_windows, window);
    }
  }

  int num_windows = g_list_length(workspace_windows);

  if (num_windows < 2) {
    exit(0);
  }

  // which direction to switch windows -- next or prev
  char *direction = "next";

  if (argc > 1) {
    direction = argv[1];
  }

  for (int i = 0; i < num_windows; i++) {
    window = WNCK_WINDOW(g_list_nth_data(workspace_windows, i));
    if (window == active_window) {
      int new = i + 1;
      if (new > num_windows - 1) {
        new = 0;
      }
      if (strcmp(direction, "prev") == 0) {
        new = i - 1;
        if (new < 0) {
          new = num_windows - 1;
        }
      }
      window = WNCK_WINDOW(g_list_nth_data(workspace_windows, new));
      guint32 timestamp =
          gdk_x11_get_server_time(gdk_x11_window_lookup_for_display(
              gdk_display_get_default(), GDK_ROOT_WINDOW()));
      wnck_window_activate(window, timestamp);
      break;
    }
  }

  g_list_free(all_windows);
  g_list_free(workspace_windows);
  return 0;
}
