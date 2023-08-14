const { dir, log } = require('console');
const { get } = require('http');
const { parse } = require('path');
const { emit } = require('process');
const readline = require('readline');

const rl = readline.createInterface({
    input: process.stdin,
    output: process.stdout,
    terminal: false
});

let input = "";

//let test = "$&hairsp;AESym.: BRKInstr.: Break / interruptAddr.: implied"
//console.log(test.substring(9, 11));
//console.log(test.match(/Sym.: (.*?)Instr.:/)[1]);
//console.log(test.match(/Addr.: (.*)/)[1]);

rl.on("line", (line) => {
    if (line === "do") {
        result = parseHtmlTable(input);
        console.log(result);
        process.exit(0);
    }
    input += line;
});

function parseHtmlTable(input) {
    const entries = getEntries(input);
    console.log(JSON.stringify(entries, null, 2))
    console.log("-----------------------------------------");
    return assembleEntries(entries);
}

function getEntries(input) {

    // search input for `<td data-description="` to the next `"` and list them.

    let result = [];

    input.split("<td data-description=\"").slice(1).forEach((element) => {
        let individual_text = element.split("\"")[0].replace("\n", "");
  
        //console.log(individual_text);
        individual_to_entry(individual_text);
        result.push(individual_to_entry(individual_text));
    });
    return result;
}

function individual_to_entry(text) {
    let address = text.substring(9, 11).trim();
    let symbol = text.match(/Sym.: (.*?)Instr.:/)[1].trim();
    let addressingMode_raw = text.match(/Addr.: (.*)/)[1].trim();

    return new Entry(symbol, addressingMode_raw, address);
}

function assembleEntries(entries) {
    let result = "enum Opcode {\n";
    for (let i = 0; i < entries.length; i++) {
        result += "     " + entries[i].toFormattedString();
        if (i < entries.length - 1) {
            result += ",\n";
            continue;
        }
        result += "\n}";
    }
    return result;
}
class Entry {
    Symbol = "";
    RawAddressingMode = "";
    RawAddress = "";

    constructor(symbol, addressingMode, address) {
        this.Symbol = symbol;
        this.RawAddressingMode = addressingMode;
        this.RawAddress = address;
    }

    toFormattedString() {
        let result = "";
        result += this.Symbol;
        if (this.RawAddressingMode !== "") {
            result += this.parseAddressingMode();
        }
        result += ` = 0x${this.RawAddress}`
        return result;
    }
    parseAddressingMode() {
        let result = "";
        let copy = this.RawAddressingMode.replace(",", "");

        let modes = copy.split(" ");

        for (let i = 0; i < modes.length; i++) {
            switch (modes[i]) {
                case "immediate":
                    result += "_IMM";
                    break;
                case "zero-page":
                    result += "_ZP";
                    break;
                case "indirect":
                    result += "_IND";
                    break;
                case "absolute":
                    result += "_ABS";
                    break;
                case "implied":
                    result += "_IMP";
                    break;
                case "X-indexed":
                    result += "_X";
                    break;
                case "Y-indexed":
                    result += "_Y";
                    break;
                case "relative":
                    result += "_REL";
                    break;
                case "accumulator":
                    result += "_ACC";
                    break;
                default:
                    console.log("Unknown addressing mode: " + modes[i]);
            }
            return result;
        }
    }
}
