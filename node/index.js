const express = require('express')
const axios = require('axios')
const app = express()
const port = 3000

app.set('views', './views')
app.set('view engine', 'ejs')

app.use(express.static('data'))

function get_query_string(query, page)
{
	return "t=" + encodeURIComponent(query) + "&p=" + page;
}

function on_result(res, args, ret)
{
	const result = ret.data;
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
	args.result = result;
	res.render('index', args);
}

function on_error(res, ex)
{
	console.log(ex.message);
	res.status(500).send(ex.message);
}

app.get('/', (req, res) => {
	const query = req.query.t;
	const page = req.query.p;
	
	let args = {};
	args.query = query;
	args.page = page;
	
	if(query) {
		console.log(req.query);
		axios.get('http://localhost:8080/search/query?' + get_query_string(query, page))
			.then(on_result.bind(null, res, args))
			.catch(on_error.bind(null, res));
	} else {
		args.items = [];
		args.result = null;
		res.render('index', args);
	}
})

app.listen(port, '0.0.0.0', () => {
	console.log(`Listening at http://0.0.0.0:${port}`)
})
