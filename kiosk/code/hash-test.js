function random_byte() {
	return Math.floor(256*Math.random());
}

function random_id() {
	return [...Array(5).keys()].map(x => random_byte());
}


function hash_fold_add(id) {
	// shift left
	const shiftL = (b, amt) => b * (2**amt);
	// shift right
	const shiftR = (b, amt) => Math.floor(b / (2**amt));
	// bottom 4 bits
	const lo = (b) => b % 16;

	return [a, b, c, d].reduce((acc, x) => acc+x, 0) % 4096;
}


function hash_fold_xor(id) {
	// shift left
	const shiftL = (b, amt) => b * (2**amt);
	// shift right
	const shiftR = (b, amt) => b % (2**amt);
	// bottom 4 bits
	const lo = (b) => b % 16;
	// upper 4 bits
	const hi = (b) => Math.floor(b/16);

	const a = shiftL(id[0], 4) + hi(id[1]);
	const b = shiftL(lo(id[1]), 8) + id[2];
	const c = shiftL(id[3], 4) + hi(id[4]);
	const d = lo(id[4]);

	return [a, b, c, d].reduce((acc, x) => acc^x, 0) % 4096;
}


const NUM_IDS = 5000000;
const arr = [...Array(NUM_IDS).keys()]
	.map(x => random_id())
	


function test_hash(name, hash, arr) {
	const n = arr.length;
	const m = 4096;

	const bins = arr.map(x => hash(x)).reduce(
		(acc, x) => {
			acc[x] += 1;
			return acc;
		},
		[...Array(m).keys()].map(x => 0)
	);
	
	const square_sum = bins.reduce((acc, b) => acc + (b*b), 0);
	const chi2 = ((m/n) * square_sum) - n

	const E = m-1;
	const diff = Math.abs(chi2-E);
	
	console.log(`${name}: ${chi2}	[${diff}]`);
}


test_hash('fold-add', hash_fold_add, arr);
test_hash('fold-xor', hash_fold_xor, arr);
