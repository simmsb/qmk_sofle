#pragma once

/* The way how "handedness" is decided (which half is which),
see https://docs.qmk.fm/#/feature_split_keyboard?id=setting-handedness
for more options.
*/

#define COMBO_ALLOW_ACTION_KEYS
#define COMBO_VARIABLE_LEN

#define RPC_M2S_BUFFER_SIZE 255
#define SPLIT_TRANSACTION_IDS_USER USER_SEND_DISP
