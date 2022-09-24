#include "../cmd.h"
#include "argtable3.h"

void blisp_flash_firmware(const char* firmware_path) {
    printf("");
}


#define REG_EXTENDED 1
#define REG_ICASE    (REG_EXTENDED << 1)

static struct arg_rex* cmd;
static struct arg_file* binary_to_write;
static struct arg_end* end;
static void* cmd_write_argtable[3];

int8_t
cmd_write_args_init() {
    cmd_write_argtable[0] = cmd
        = arg_rex1(NULL, NULL, "write", NULL, REG_ICASE, NULL);
    cmd_write_argtable[1] = binary_to_write
        = arg_file1(NULL, NULL, "<input>", "Binary to write");
    cmd_write_argtable[2] = end = arg_end(10);

    if (arg_nullcheck(cmd_write_argtable) != 0) {
        fprintf(stderr, "insufficient memory\n");
        return -1;
    }
    return 0;
}

void cmd_write_args_print_glossary() {
    fputs("Usage: blisp", stdout);
    arg_print_syntax(stdout,cmd_write_argtable,"\n");
    puts("Writes firmware to SPI Flash");
    arg_print_glossary(stdout,cmd_write_argtable,"  %-25s %s\n");
}

uint8_t
cmd_write_parse_exec(int argc, char** argv) {
    int errors = arg_parse(argc, argv, cmd_write_argtable);
    if (errors == 0) {
        blisp_flash_firmware(binary_to_write->filename[0]); // TODO: Error code?
        return 1;
    } else if (cmd->count == 1) {
        cmd_write_args_print_glossary();
        return 1;
    }
    return 0;
}

void cmd_write_args_print_syntax() {
    arg_print_syntax(stdout,cmd_write_argtable,"\n");
}

void
cmd_write_free() {
    arg_freetable(cmd_write_argtable,
                  sizeof(cmd_write_argtable) / sizeof(cmd_write_argtable[0]));
}

struct cmd cmd_write
    = { "write", cmd_write_args_init, cmd_write_parse_exec, cmd_write_args_print_syntax, cmd_write_free };