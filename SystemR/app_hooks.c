#include <stdio.h>
#include "ui_utils.h"
#include "ui_dialog.h"

void vApplicationMallocFailedHook() {
    ui_dialog_alert("Fatal Error", "SYS ASSERT: Out of Memory.");
    printf("SYS ASSERT: Out of Memory.\n");
    while (1) ;
}
