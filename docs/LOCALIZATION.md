# Interface language and keyboard navigation

Open **View → Preferences... → Language** and choose English or Português
(Brasil). Apply saves the preference for the next opening. The current source,
machine, history and editor Undo remain available; no restart is forced. Closing
the dialog without Apply discards the selection. An invalid saved language uses
English. `--ui-language en` or `--ui-language pt-BR` overrides it for one launch.

Menus, controls, common dialogs, statuses and accessibility names use the bundled
Portuguese catalog. The technical help/manuals, compiler diagnostics and some
detailed analysis/error descriptions retain English. Code, instruction names,
numbers, paths, command identifiers and project formats retain their meaning.
There is no system locale change or external translation service.

Use Ctrl+Alt+M to focus the menu bar. Arrow keys navigate; Escape closes a menu;
Tab/Shift+Tab leave the bar. Ctrl+Tab/Ctrl+Shift+Tab navigate workspace panels.
Context help buttons participate in Tab navigation. Save/Discard/Cancel prompts
keep focus inside their choices and restore the previous editor focus afterward.
