const Mustache = require('mustache');
const fs = require('fs');
const df = require('dateformat');
const csv = require('csv-parser');


let applyTemplate = function() {

    // process fields
    for(let e of Object.values(types.structs)) {

        if(e.fields !== undefined) {

            // get fields and convert to array
            let fields = e.fields;
            e.fields = [];

            for (let [key, field] of Object.entries(fields)) {

                // row init to size if size is set
                field.isArray = !!field.size;
                field.size = field.isArray ? field.size.split(',') : [];

                // size list
                field.sizeLst = '';
                if(field.size.length > 0) {
                    field.sizeLst = '[' + field.size.join('][') + ']';
                    field.sizes = '{' + field.size.join(',') + '}';
                } else
                    field.size = undefined;

                // add type
                field.isStruct = types.structs[field.type] !== undefined;
                field.isEnum = types.enums[field.type] !== undefined;
                field.isBase = !field.isStruct && !field.isEnum;

                // set last one
                field.last = false;

                // create field and add
                field.name = key;
                e.fields.push(field);

            }

            // set to be last
            e.fields[e.fields.length - 1].last = true;

        }
    }

    for(let e of config.generation) {

        // read template and save
        fs.readFile(e.template, "utf8", function(err, template) {

            // get results
            content.filename = e.target;
            content.includeGuard = e.includeGuard;
            content.headerFile = e.headerFile;

            // render
            let result = Mustache.render(template, content);

            // read template and
            fs.writeFile(e.target, result, function(err) {
                if(err)
                    console.log(err);
            });

        });

    }

};




// get arguments
var args = process.argv.slice(2);
if(args.length !== 1) {
    console.error("please provide path to config");
    process.exit()
}

// read json file
let config = JSON.parse(fs.readFileSync(args[0]));
let content = config.content;
let types = {};

// update content
content.date = df(new Date(), "yyyy-mm-dd"); // date
content.year = df(new Date(), "yyyy"); // year
content.classDescription = content.classDescription.split("\n").join("\n * ");

// set enums
if(content.enums)
    content.enums = content.enums.map((e) => { e.list = "{ " + e.list.join(', ') + " }"; return e; });
else
    content.enums = [];

// save types
types.enums = {}; types.structs = {};
content.enums.map((e) => { types.enums[e.name] = e; });
content.structs.map((e) => { types.structs[e.name] = e;});

// update attributes default
content.attributes.map((e) => {

    // set last flag
    e.last = false;

    // strings
    if(e.type === "string") {
        e.type = "std::string";
        content.includes.push("string");
    }

    // default
    if(e.default !== undefined)
        e.default = e.default.toString();

    return e;

});
content.attributes[content.attributes.length - 1].last = true;

// unique includes
content.includes.filter((v, i, a) => a.indexOf(v) === i);

// read fields
let fields = {};
if(typeof config.fields === "string") {

    fs.createReadStream(config.fields)
        .pipe(csv())
        .on('data', (row) => {

            if(types.structs[row.struct] === undefined) {
                console.error("Struct " + row.struct + " is not defined");
                return;
            }

            if(types.structs[row.struct].fields === undefined)
                types.structs[row.struct].fields = {};

            types.structs[row.struct].fields[row.name] = row;

        })
        .on('end', () => {

            applyTemplate();

        });

} else {

    applyTemplate();

}
