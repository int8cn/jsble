
var myChart;
function createChart()
{	
	myChart = echarts.init(document.getElementById('main_1'));
	// 基于准备好的dom，初始化echarts实例
	option = {
		title: {
			text: '加速度传感器',
			left: 'left'
		},
		legend: {
			left: 'right',
			data: ['x轴', 'y轴','z轴']
		},
		xAxis: {
			type: 'category',
			axisLine: {
				width: 5,
				color: 'rgb(128, 128, 128)',
				},
			splitLine: {
				show: false
					},
			data:[]
		},
		yAxis: {
			type: 'value',
			axisLine: {
				width: 5,
				onZero: false,
				color: 'rgb(128, 128, 128)',
				},
		},
		series: [
			{
				name: 'x轴',
				type:'line',
				smooth:false,
				symbol: 'none',
				hoverAnimation: false,
				itemStyle: {
					normal: {
												
							lineStyle: {
							  width:0.8
							},
							color: 'rgb(255, 70, 131)'
					}
				},
				data: []
			},
			{
				name: 'y轴',
				type:'line',
				smooth:false,
				symbol: 'none',
				hoverAnimation: false,
				itemStyle: {
					normal: {
												
							lineStyle: {
							  width:0.8
							},
							color: 'rgb(0, 201, 87)'
					}
				},
				data: []
			},
			{
				name: 'z轴',
				type:'line',
				smooth:false,
				symbol: 'none',
				hoverAnimation: false,
				itemStyle: {
					normal: {
												
							lineStyle: {
							  width:0.8
							},
							color: 'rgb(30, 144, 255)'
					}
				},
				data: []
			}
		]
	};
	myChart.setOption(option);
}	

function setTile(tile) {

	option.title.text = tile;
}
function addData(value,xMax) {
	var now = new Date();
	for(var i=0;i<value.length;i++)
	{		
		if(option.series[i].data.length >= xMax) 
		{		
			option.series[i].data.shift();
			option.xAxis.data.shift(); 
		}
		option.series[i].data.push(value[i]);
		option.xAxis.data.push(now.getMinutes()+':'+now.getSeconds()+':'+now.getMilliseconds());
	}
	myChart.setOption(option);
}
$(document).ready(function(){
		createChart();
		
});
function hex2signedDec(hex) {
	try {
		var x = new BigNumber(hex, 16);
	}
	catch(err) {
		return null;
	}
	if( x.isInt() && x.gte(0) ) {
		if( hex.length==2 && x.gte(128) ) x=x.minus(256);
		if( hex.length==4 && x.gte(32768) ) x=x.minus(65536);
		if( hex.length==8 && x.gte(2147483648) ) x=x.minus(4294967296);
		if( hex.length==2 || hex.length==4 || hex.length==8 )
			return x.toString(10);
			else
		 return null;
	}
	else
		return null;
}
function jsCallback(jsonStr){ 
	if(jsonStr.cmd == 'ON_LE_SCAN')
	{
		var json = JSON.parse(jsonStr.prm);
		if(json.mac != 'FF:FF:FF:FF:FF:FF')
		{
		    return;
		}
		var x = json.scanData.substr(4,4);
		var y = json.scanData.substr(8,4);
		var z = json.scanData.substr(12,4);
		x = hex2signedDec(x);
		y = hex2signedDec(y);
		z = hex2signedDec(z);
		$("#info_label").text('x:'+x+' y:'+y+' z:'+z);
		addData([x,y,z],100);
	}
}
function randomData() {
	var value = Math.round(Math.random() * 1000);
	addData([value*2,value/5,value*10-50],10);
}