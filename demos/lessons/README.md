# Guided exercise sources

These files are the starting programs for **Tools > Guided exercises** in the
classic workspace. Most intentionally contain a small mistake for the learner to
find. A successful build is expected; an initially failing exercise check is part
of the lesson. Keep both the starting result and the corrected result covered by
the exercise tests.

Edit the C files directly. `studio/lessons.cpp` supplies the task, hint, machine
settings and expected outcome. `studio/resources.qrc` bundles the source for the
preview and editor, so the installed application does not need this source tree
to display the exercise. The resource tests compare the embedded text with these
maintained files.

Run `make format-check` from the project root after edits. If a lesson changes,
update its meaningful before/after cases in `tests/studio/test_exercise.cpp` and
any affected workspace regression. Keep lesson IDs stable so saved projects can
still identify the exercise.
