#ifndef LOAD_SAVE_H
#define LOAD_SAVE_H

#ifdef MEMORY
void save_mode();
#endif

#ifdef CONFIG_MODE
void save_state();
#else
#define save_state save_mode
#endif

#if defined(MEMORY) || defined(CONFIG_MODE)
void restore_state();
#endif

#endif

