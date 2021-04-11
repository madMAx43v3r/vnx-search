const express = require('express')
const axios = require('axios')
const {createProxyMiddleware} = require('http-proxy-middleware');
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
	const num_pages = Math.ceil(result.num_results_total / result.options.limit);
	if(args.page > 0) {
		args.page_list.push({
			href: "?" + get_query_string(args.query, args.page - 1),
			text: "Prev"
		});
	}
	for(let i = 0; i < num_pages;) {
		if(i != args.page) {
			args.page_list.push({
				href: "?" + get_query_string(args.query, i),
				text: `${i+1}`
			});
		} else {
			args.page_list.push({
				text: `${i+1}`
			});
		}
		if(i < 9) {
			i++;
		} else {
			i += 10;
		}
	}
	if(args.page + 1 < num_pages) {
		args.page_list.push({
			href: "?" + get_query_string(args.query, args.page + 1),
			text: "Next"
		});
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
	args.body = 'result';
	args.query = query;
	args.page = parseInt(page);
	args.result = null;
	args.page_list = [];
	
	if(query) {
		console.log(req.query);
		axios.get('http://localhost:8080/search/query?' + get_query_string(query, page))
			.then(on_result.bind(null, res, args))
			.catch(on_error.bind(null, res));
	} else {
		res.render('index', args);
	}
})

app.get('/page_info', (req, res) => {
	const url_key = req.query.k;
	if(url_key) {
		axios.get('http://localhost:8080/search/get_page_info?k=' + url_key)
			.then((ret) => {
				let args = {};
				args.body = 'page_info';
				args.url_key = url_key;
				args.data = ret.data;
				res.render('index', args);
			}).catch(on_error.bind(null, res));
	} else {
		res.status(404).send();
	}
})

app.use('/archive', createProxyMiddleware({target: 'http://localhost:8081', changeOrigin: true}));

app.listen(port, '0.0.0.0', () => {
	console.log(`Listening at http://0.0.0.0:${port}`);
})
