import * as functions from 'firebase-functions';
import * as admin from 'firebase-admin';

admin.initializeApp(functions.config().firebase);
const db = admin.firestore();
const settingsCollection = db.collection('settings');

export const setServerURL = functions.https.onRequest(async (req, res) => {
    const serverSettingsDoc = settingsCollection.doc(req.query.user);
    await serverSettingsDoc.set({
        url: req.query.url,
    });
    res.sendStatus(200);
});

export const getServerURL = functions.https.onRequest(async (req, res) => {
    const serverSettingsDoc = settingsCollection.doc(req.query.user);
    const serverSettings = await serverSettingsDoc.get();
    res.send(serverSettings.get('url'));
});

