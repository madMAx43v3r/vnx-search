const express = require('express')
const http = require('http')
const app = express()
const port = 3000

app.set('views', './views')
app.set('view engine', 'ejs')

app.use(express.static('data'))

function get_query(query, page)
{
	return "t=" + encodeURIComponent(query) + "&p=" + page;
}

function on_result(res, args, json)
{
	const result = JSON.parse(json);
	for(const item of result.items)
	{
		let date = new Date(item.last_modified * 1000);
		let url_str = item.url.replace(/(^\w+:|^)\/\//, '');
		if(!item.title.length) {
			item.title = url_str;
		}
		item.url_str = url_str;
		item.date_str = date.toDateString();
	}
//	console.log(result);
	args.result = result;
	res.render('index', args);
}

app.get('/', (req, res) => {
	const query = req.query.t;
	const page = req.query.p;
	
	let args = {};
	args.query = query;
	args.page = page;
	
	if(query) {
		console.log(`Got query="${query}", page=${page}`);
		const sreq = http.request({
				method: 'GET', hostname: 'localhost', port: 8080,
				path: '/search/query?' + get_query(query, page)
			}, (sres) => {
				sres.on('data', on_result.bind(null, res, args));
			})
		sreq.end();
	} else {
		args.items = [];
		args.result = null;
		res.render('index', args);
	}
})

app.listen(port, '0.0.0.0', () => {
	console.log(`Listening at http://0.0.0.0:${port}`)
})
