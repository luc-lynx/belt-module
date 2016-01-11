/*
 * Quick & dirty testing module for Belt algotithm implementations.
 * Some ideas were taken from tcrypt module. Test vectors are from STB 34.101.31.
 *
 * Copyright (c) 2016 Evgeny Sidorov <evgenij.sidorov@gmail.com>
 * 
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 * 
 */


#include <linux/err.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/crypto.h>
#include <linux/slab.h>
#include <linux/scatterlist.h>
#include <linux/string.h>

const u8 XEnc[16] =
{
	0xB1, 0x94, 0xBA, 0xC8, 0x0A, 0x08, 0xF5, 0x3B, 
	0x36, 0x6D, 0x00, 0x8E, 0x58, 0x4A, 0x5D, 0xE4
};
    
const u8 KeyEnc[32] =
{
	0xE9, 0xDE, 0xE7, 0x2C,	0x8F, 0x0C, 0x0F, 0xA6,
	0x2D, 0xDB, 0x49, 0xF4,	0x6F, 0x73, 0x96, 0x47,
	0x06, 0x07, 0x53, 0x16,	0xED, 0x24, 0x7A, 0x37,
	0x39, 0xCB, 0xA3, 0x83,	0x03, 0xA9, 0x8B, 0xF6
};

const u8 YEnc[16] = 
{
	0x69, 0xCC, 0xA1, 0xC9, 0x35, 0x57, 0xC9, 0xE3,
	0xD6, 0x6B, 0xC3, 0xE0, 0xFA, 0x88, 0xFA, 0x6E
};
    
const u8 XDec[16] =
{
	0xE1, 0x2B, 0xDC, 0x1A,	0xE2, 0x82, 0x57, 0xEC,
	0x70, 0x3F, 0xCC, 0xF0,	0x95, 0xEE, 0x8D, 0xF1
};
    
const u8 KeyDec[32] =
{
	0x92, 0xBD, 0x9B, 0x1C,	0xE5, 0xD1, 0x41, 0x01,
	0x54, 0x45, 0xFB, 0xC9,	0x5E, 0x4D, 0x0E, 0xF2,
	0x68, 0x20, 0x80, 0xAA,	0x22, 0x7D, 0x64, 0x2F,
	0x26, 0x87, 0xF9, 0x34,	0x90, 0x40, 0x55, 0x11
};

const u8 YDec[16] = 
{
	0x0D, 0xC5, 0x30, 0x06, 0x00, 0xCA, 0xB8, 0x40,
	0xB3, 0x84, 0x48, 0xE5, 0xE9, 0x93, 0xF4, 0x21
};

/* the function was taken from tcrypt.c module */
static void hexdump(unsigned char *buf, unsigned int len)
{
        print_hex_dump(KERN_CONT, "", DUMP_PREFIX_OFFSET,
                        16, 1,
                        buf, len, false);
}


static int __init belt_test_mod_init(void)
{
	struct crypto_cipher *tfm;
	const char *alg = "belt";
	const char *driver = "belt-generic";
	const u32 type = 0;
	const u32 mask = 0;
	int ret = -ENOMEM;
	u8 tfmres[16] = {0};

	printk("belt_test_mod_init called\n");

	tfm = crypto_alloc_cipher(driver, type, mask);
	if (IS_ERR(tfm)) {
                printk(KERN_ERR "alg: cipher: Failed to load transform for "
                       "%s: %ld\n", driver, PTR_ERR(tfm));
                return -EAGAIN;
        }

	crypto_cipher_clear_flags(tfm, ~0);
	
	ret = crypto_cipher_setkey(tfm, KeyEnc, sizeof(KeyEnc));
	if(ret != 0)
	{
		printk(KERN_ERR "crypto_cipher_setkey failed for %s, retcode %d\n", alg, ret);
		goto out;
	}

	printk("%s block size %d\n", alg, crypto_cipher_blocksize(tfm));

	crypto_cipher_encrypt_one(tfm, tfmres, XEnc);

	if(memcmp(YEnc, tfmres, sizeof(tfmres)))
	{
		printk(KERN_ERR "%s cipher encryption test failed\n", alg);
		hexdump(tfmres, sizeof(tfmres));
		goto out;
	}	
	else
	{
		printk("%s passed encryption test\n", alg);
	}

	crypto_cipher_clear_flags(tfm, ~0);

	ret = crypto_cipher_setkey(tfm, KeyDec, sizeof(KeyDec));
	if(ret != 0)
	{
		printk(KERN_ERR "crypto_cipher_setkey failed for %s, retcode %d\n", alg, ret);
		goto out;
	}

	crypto_cipher_decrypt_one(tfm, tfmres, XDec);

	if(memcmp(YDec, tfmres, sizeof(tfmres)))
	{
		printk(KERN_ERR "%s cipher decryption test failed\n", alg);
		hexdump(tfmres, sizeof(tfmres));
		goto out;
	}
	else
	{
		printk("%s passed decryption test\n", alg);
	}

out:
	crypto_free_cipher(tfm);
	return -EAGAIN;
}

static void __exit belt_test_mod_fini(void){ }

module_init(belt_test_mod_init);
module_exit(belt_test_mod_fini);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Quick & dirty Belt testing module");
MODULE_AUTHOR("Evgeny Sidorov <evgenij.sidorov@gmail.com>");
   
