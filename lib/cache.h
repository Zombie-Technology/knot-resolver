/*  Copyright (C) 2014 CZ.NIC, z.s.p.o. <knot-dns@labs.nic.cz>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <libknot/rrset.h>
#include <libknot/internal/namedb/namedb.h>

/*!
 * \brief Serialized form of the RRSet with inception timestamp.
 */
struct kr_cache_rrset
{
	uint32_t timestamp;
	uint16_t count;
	uint8_t  data[];
};

/*!
 * \brief Open/create persistent cache in given path.
 * \param handle Path to existing directory where the DB should be created.
 * \param mm Memory context.
 * \return database instance or NULL
 */
namedb_t *kr_cache_open(const char *handle, mm_ctx_t *mm);

/*!
 * \brief Close persistent cache.
 * \note This doesn't clear the data, just closes the connection to the database.
 * \param cache database instance
 */
void kr_cache_close(namedb_t *cache);

/*!
 * \brief Begin cache transaction (read-only or write).
 *
 * \param cache database instance
 * \param txn transaction instance to be initialized (output)
 * \param flags transaction flags (see namedb.h in libknot)
 * \return KNOT_E*
 */
int kr_cache_txn_begin(namedb_t *cache, namedb_txn_t *txn, unsigned flags);


/*!
 * \brief Commit existing transaction.
 * \param txn transaction instance
 * \return KNOT_E*
 */
int kr_cache_txn_commit(namedb_txn_t *txn);

/*!
 * \brief Abort existing transaction instance.
 * \param txn transaction instance
 */
void kr_cache_txn_abort(namedb_txn_t *txn);

/*!
 * \brief Query the cache for given RRSet (name, type, class)
 * \note The 'drift' is the time passed between the cache time of the RRSet and now (in seconds).
 * \param txn transaction instance
 * \param rr query RRSet (its rdataset may be changed depending on the result)
 * \param timestamp current time (will be replaced with drift if successful)
 * \return KNOT_E*
 */
int kr_cache_query(namedb_txn_t *txn, knot_rrset_t *rr, uint32_t *timestamp);

/*!
 * \brief Insert RRSet into cache, replacing any existing data.
 * \param txn transaction instance
 * \param rr inserted RRSet
 * \param timestamp current time
 * \return KNOT_E*
 */
int kr_cache_insert(namedb_txn_t *txn, const knot_rrset_t *rr, uint32_t timestamp);

/*!
 * \brief Remove RRSet from cache.
 * \param txn transaction instance
 * \param rr removed RRSet
 * \return KNOT_E*
 */
int kr_cache_remove(namedb_txn_t *txn, const knot_rrset_t *rr);
