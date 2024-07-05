#include <iostream>
#include <fstream>
#include <vector>
#include <filesystem>
#include <xed/xed-interface.h>
#include <nlohmann/json.hpp> // Include the nlohmann/json header file

using json = nlohmann::json;
namespace fs = std::filesystem;

// Function to decode an instruction and return JSON representation
json decode_instruction(xed_decoded_inst_t* xedd) {
    // Create a JSON object to store the decoded information
    json j;

    // Get the instruction mnemonic
    char buffer[256];
    xed_decoded_inst_dump(xedd, buffer, sizeof(buffer));
    j["mnemonic"] = std::string(buffer);

    // Get operand information
    unsigned int num_operands = xed_decoded_inst_noperands(xedd);
    j["num_operands"] = num_operands;

    for (unsigned int i = 0; i < num_operands; ++i) {
        const xed_operand_t* op = xed_decoded_inst_operand(xedd, i);
        json operand;
        operand["name"] = xed_operand_name(op);
        operand["type"] = xed_operand_type_enum_t2str(xed_operand_type(op));
        j["operands"].push_back(operand);
    }

    // Additional information
    j["iform"] = xed_iform_enum_t2str(xed_decoded_inst_get_iform_enum(xedd));
    j["category"] = xed_category_enum_t2str(xed_decoded_inst_get_category(xedd));
    j["extension"] = xed_extension_enum_t2str(xed_decoded_inst_get_extension(xedd));
    j["isa_set"] = xed_isa_set_enum_t2str(xed_decoded_inst_get_isa_set(xedd));

    return j;
}

int main() {
    // Initialize the XED tables
    xed_tables_init();

    // Create output directory if it doesn't exist
    fs::create_directory("output");

    // Iterate over all iforms
    for (int i = 0; i < XED_IFORM_LAST; ++i) {
        xed_decoded_inst_t xedd;
        xed_state_t dstate;

        // Set the machine mode and address width
        xed_state_zero(&dstate);
        dstate.mmode = XED_MACHINE_MODE_LONG_64;
        dstate.stack_addr_width = XED_ADDRESS_WIDTH_64b;

        // Initialize the decoder request
        xed_decoded_inst_zero_set_mode(&xedd, &dstate);

        // Get the itext for the iform
        xed_uint8_t itext[XED_MAX_INSTRUCTION_BYTES];
        unsigned int bytes = xed_build_one_byte_from_iform_enum(static_cast<xed_iform_enum_t>(i), itext, XED_MAX_INSTRUCTION_BYTES);

        // Decode the instruction
        xed_error_enum_t xed_error = xed_decode(&xedd, itext, bytes);

        if (xed_error == XED_ERROR_NONE) {
            // Decode the instruction and get the JSON representation
            json decoded_json = decode_instruction(&xedd);

            // Write each decoded instruction to a separate JSON file
            std::ofstream output_file("output/decoded_instruction_" + std::to_string(i) + ".json");
            if (output_file.is_open()) {
                output_file << decoded_json.dump(4); // Pretty print with 4 spaces
                output_file.close();
                std::cout << "Decoded instruction " << i << " written to output/decoded_instruction_" << i << ".json" << std::endl;
            } else {
                std::cerr << "Unable to open file for writing" << std::endl;
                return 1;
            }
        }
    }

    return 0;
}
