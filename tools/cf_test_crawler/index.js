#!/usr/bin/env node
"use strict";

const fs = require("fs");
const util = require("util");
const assert = require("assert");
const request = require("request");
const cheerio = require("cheerio");

const problem_url = "http://codeforces.com/problemset/problem/%s/%s";
const contest_url = "http://codeforces.com/contest/%s/problem/%s";
const gym_url = "http://codeforces.com/gym/%s/problem/%s"

function parse_body(body) {
    let $ = cheerio.load(body);
    // console.log($('.sample-tests').html());
    // console.log($('.sample-tests').text());

    fs.mkdirSync('test');
    let tests = $('.sample-test');
    for (let i = 0; i < tests.length; ++i) {
        let test = tests[i];
        let input = $('.input pre', test);
        let output = $('.output pre', test);
        // console.log(input[0]);
        $('br', input).replaceWith('\n');
        $('br', output).replaceWith('\n');

        if (input.length != output.length) {
            console.log("input and output mismatch");
            return;
        }

        for (let j = 0; j < input.length; ++j) {
            let intext = input.eq(j).text();
            let outtext = output.eq(j).text();
            let infix = (tests.length == 1) ? "" : "-${i}";
            if (intext[intext.length - 1] != '\n')
                intext += '\n';
            if (outtext[outtext.length - 1] != '\n')
                outtext += '\n';
            fs.writeFileSync(`test/in${infix}-${j}.txt`, intext);
            fs.writeFileSync(`test/out${infix}-${j}.txt`, outtext);
        }
    }
    console.log(`Finished`);
}

function crawl_problem(url) {
    let arg = {url: url, followRedirect: false};
    request(arg, function(err, res, body) {
        if (err) {
            console.log("http request failed");
            return;
        }

        if (res.statusCode != 200) {
            console.log(`http status(${res.statusCode}) != OK`);
            return;
        }
        parse_body(body);
    });
}

function main() {
    const argv = require("minimist")(process.argv.slice(2));
    var url;
    if (argv.c) {
        assert(argv._.length == 1);
        url = util.format(contest_url, argv.c, argv._[0]);
    } else if (argv.p) {
        assert(argv._.length == 1);
        url = util.format(problem_url, argv.p, argv._[0]);
    } else if (argv.g) {
        assert(argv._.length == 1);
        url = util.format(gym_url, argv.g, argv._[0]);
    } else if (argv.u) {
        url = argv.u
    } else {
        console.log("Usage :");
        console.log("\tcf_test_crawler -p/c CID P");
        console.log("\tcf_test_crawler -u url");
        console.log("\tcf_test_crawler -g GYM P");
        return;
    }
    console.log(`Crawl from ${url}`);
    crawl_problem(url);
}
main()
